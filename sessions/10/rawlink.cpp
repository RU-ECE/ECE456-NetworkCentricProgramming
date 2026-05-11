/*
 * rawlink.cpp — Raw Ethernet 0x88B5: send and recv via plain AF_PACKET
 *
 * Wire format (Ethernet payload):
 *   [FROM:2][TO:2][PACKET_ID:4][payload...]
 *   Total header: 8 bytes
 *
 * EtherType: 0x88B5 (locally assigned, experimentation)
 * MAC: broadcast ff:ff:ff:ff:ff:ff (or peer MAC via RAWLINK_PEER_IF)
 *
 * Build:
 *   g++ -std=c++17 -O2 -o rawlink rawlink.cpp
 *
 * SOCK_RAW needs CAP_NET_RAW. Avoid sudo on every run (one-time, still as root):
 *   sudo setcap cap_net_raw+ep ./rawlink
 *
 * Usage:
 *   ./rawlink <iface> <my_addr> send <dst_addr> <message>
 *   ./rawlink <iface> <my_addr> recv
 *
 * Optional: RAWLINK_PEER_IF=veth1 — use that interface's MAC as Ethernet dst.
 *
 * Example (two terminals, same machine with veth pair):
 *   sudo ip link add veth0 type veth peer name veth1
 *   sudo ip link set veth0 up
 *   sudo ip link set veth1 up
 *
 *   Terminal 1:  sudo ./rawlink veth1 2 recv
 *   Terminal 2:  sudo ./rawlink veth0 1 send 2 "hello world"
 */

#include <arpa/inet.h>
#include <cerrno>
#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <linux/if_ether.h>
#include <linux/if_packet.h>
#include <net/if.h>
#include <sched.h>
#include <sstream>
#include <stdexcept>
#include <string>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>
#include <vector>

using namespace std;

/* ------------------------------------------------------------------ */
/* Constants                                                            */
/* ------------------------------------------------------------------ */

static constexpr uint16_t ETHERTYPE_RAWLINK = 0x88B5;
static constexpr size_t RAWLINK_HDR_SIZE = 8;
static constexpr size_t ETH_HDR_SIZE = 14;
static constexpr size_t MAX_PAYLOAD = 1500 - RAWLINK_HDR_SIZE;
static constexpr size_t RAWLINK_ETH_MIN = 60;

static const uint8_t BCAST_MAC[6] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};

/* ------------------------------------------------------------------ */
/* Helpers                                                              */
/* ------------------------------------------------------------------ */

[[noreturn]] static void throw_errno(const string& msg) {
	throw runtime_error(msg + ": " + strerror(errno));
}

static uint64_t now_ns() {
	struct timespec ts;
	clock_gettime(CLOCK_MONOTONIC, &ts);
	return static_cast<uint64_t>(ts.tv_sec) * 1'000'000'000ULL + static_cast<uint64_t>(ts.tv_nsec);
}

static string mac_str(const uint8_t m[6]) {
	ostringstream oss;
	oss << hex;
	for (int i = 0; i < 6; ++i) {
		if (i)
			oss << ':';
		oss << static_cast<int>(m[i]);
	}
	return oss.str();
}

static bool hwaddr_for_iface(const char* ifname, uint8_t out[6]) {
	int s = ::socket(AF_INET, SOCK_DGRAM, 0);
	if (s < 0)
		return false;
	struct ifreq ifr {};
	strncpy(ifr.ifr_name, ifname, IFNAMSIZ - 1);
	bool ok = ::ioctl(s, SIOCGIFHWADDR, &ifr) == 0;
	if (ok)
		memcpy(out, ifr.ifr_hwaddr.sa_data, 6);
	::close(s);
	return ok;
}

static void ether_dst_mac(uint8_t dst[6]) {
	const char* peer = getenv("RAWLINK_PEER_IF");
	if (peer && peer[0] && hwaddr_for_iface(peer, dst))
		return;
	memcpy(dst, BCAST_MAC, 6);
}

/* ------------------------------------------------------------------ */
/* On-wire header                                                       */
/* ------------------------------------------------------------------ */

struct __attribute__((packed)) RawlinkHdr {
	uint16_t from;
	uint16_t to;
	uint32_t packet_id;
};

static_assert(sizeof(RawlinkHdr) == RAWLINK_HDR_SIZE, "header size mismatch");

/* ------------------------------------------------------------------ */
/* RawLink                                                              */
/* ------------------------------------------------------------------ */

class RawLink {
public:
	RawLink(const string& iface, uint16_t my_addr) : my_addr_(my_addr) { open(iface); }
	~RawLink() {
		if (fd_ >= 0)
			::close(fd_);
	}

	RawLink(const RawLink&) = delete;
	RawLink& operator=(const RawLink&) = delete;

	void send(uint16_t dst, uint32_t pkt_id, const void* payload, size_t plen) {
		if (plen > MAX_PAYLOAD)
			throw invalid_argument("payload too large");

		uint8_t edst[6];
		ether_dst_mac(edst);

		sockaddr_ll sll{};
		sll.sll_family = AF_PACKET;
		sll.sll_protocol = htons(ETHERTYPE_RAWLINK);
		sll.sll_ifindex = ifindex_;
		sll.sll_halen = 6;
		memcpy(sll.sll_addr, edst, 6);

		uint8_t buf[ETH_HDR_SIZE + RAWLINK_HDR_SIZE + 1500]{};
		memcpy(buf + 0, edst, 6);
		memcpy(buf + 6, src_mac_, 6);
		buf[12] = (ETHERTYPE_RAWLINK >> 8) & 0xff;
		buf[13] = ETHERTYPE_RAWLINK & 0xff;

		auto* hdr = reinterpret_cast<RawlinkHdr*>(buf + ETH_HDR_SIZE);
		hdr->from = htons(my_addr_);
		hdr->to = htons(dst);
		hdr->packet_id = htonl(pkt_id);
		memcpy(buf + ETH_HDR_SIZE + RAWLINK_HDR_SIZE, payload, plen);

		size_t body = ETH_HDR_SIZE + RAWLINK_HDR_SIZE + plen;
		size_t tot = body < RAWLINK_ETH_MIN ? RAWLINK_ETH_MIN : body;

		ssize_t n = ::sendto(fd_, buf, tot, 0, reinterpret_cast<sockaddr*>(&sll), sizeof(sll));
		if (n < 0)
			throw_errno("sendto");
	}

	void recv_loop() {
		cout << "[rawlink] listening on addr=" << my_addr_ << " ...\n";

		uint8_t buf[2048];
		uint64_t total_bytes = 0;
		uint32_t total_pkts = 0;
		auto t0 = chrono::high_resolution_clock::now();
		bool started = false;

		for (;;) {
			ssize_t n = ::recvfrom(fd_, buf, sizeof(buf), 0, nullptr, nullptr);
			if (n < 0) {
				if (errno == EINTR)
					continue;
				throw_errno("recvfrom");
			}
			if (static_cast<size_t>(n) < ETH_HDR_SIZE + RAWLINK_HDR_SIZE)
				continue;

			uint8_t* eth = buf;
			uint16_t etype = static_cast<uint16_t>((eth[12] << 8) | eth[13]);
			if (etype != ETHERTYPE_RAWLINK)
				continue;

			auto* hdr = reinterpret_cast<RawlinkHdr*>(eth + ETH_HDR_SIZE);
			uint16_t from = ntohs(hdr->from);
			uint16_t to = ntohs(hdr->to);
			uint32_t pktid = ntohl(hdr->packet_id);

			if (to != my_addr_ && to != 0)
				continue;

			size_t plen = static_cast<size_t>(n) - ETH_HDR_SIZE - RAWLINK_HDR_SIZE;

			if (!started) {
				t0 = chrono::high_resolution_clock::now();
				started = true;
			}
			++total_pkts;
			total_bytes += plen;

			// Print first packet and periodic stats; Ctrl-C to stop.
			if (total_pkts == 1) {
				const char* p = reinterpret_cast<const char*>(hdr) + RAWLINK_HDR_SIZE;
				cout << "[rx] first pkt from=" << from << " pkt=" << pktid << " len=" << plen << " | "
						  << string(p, min(plen, size_t(40))) << '\n';
			}
			if ((total_pkts & 0xfff) == 0) {
				auto now = chrono::high_resolution_clock::now();
				double ms = chrono::duration<double, milli>(now - t0).count();
				double mbps = (total_bytes / (1024.0 * 1024.0)) / (ms / 1000.0);
				cout << "[rx] " << total_pkts << " pkts, " << total_bytes << " B, " << ms << " ms, " << mbps
						  << " MiB/s\n";
			}
		}
	}

private:
	void open(const string& iface) {
		fd_ = socket(AF_PACKET, SOCK_RAW, htons(ETHERTYPE_RAWLINK));
		if (fd_ < 0)
			throw_errno("socket");

		struct ifreq ifr {};
		strncpy(ifr.ifr_name, iface.c_str(), IFNAMSIZ - 1);
		if (ioctl(fd_, SIOCGIFINDEX, &ifr) < 0)
			throw_errno("SIOCGIFINDEX");
		ifindex_ = ifr.ifr_ifindex;
		if (ioctl(fd_, SIOCGIFHWADDR, &ifr) < 0)
			throw_errno("SIOCGIFHWADDR");
		memcpy(src_mac_, ifr.ifr_hwaddr.sa_data, 6);

		cout << "[rawlink] iface=" << iface << " ifindex=" << ifindex_ << " mac=" << mac_str(src_mac_)
				  << " addr=" << my_addr_ << '\n';

		sockaddr_ll sll{};
		sll.sll_family = AF_PACKET;
		sll.sll_protocol = htons(ETHERTYPE_RAWLINK);
		sll.sll_ifindex = ifindex_;
		if (bind(fd_, reinterpret_cast<sockaddr*>(&sll), sizeof(sll)) < 0)
			throw_errno("bind");
	}

	int fd_ = -1;
	int ifindex_ = 0;
	uint16_t my_addr_;
	uint8_t src_mac_[6]{};
};

/* ------------------------------------------------------------------ */
/* main                                                                 */
/* ------------------------------------------------------------------ */

int main(int argc, char** argv) {
	try {
		if (argc < 4) {
			cerr << "usage:\n"
					  << "  " << argv[0] << " <iface> <my_addr> send <dst> <message>\n"
					  << "  " << argv[0] << " <iface> <my_addr> send <dst> <nbytes>   (bulk)\n"
					  << "  " << argv[0] << " <iface> <my_addr> recv\n";
			return 1;
		}

		const string iface = argv[1];
		const uint16_t my_addr = static_cast<uint16_t>(stoi(argv[2]));
		const string mode = argv[3];

		RawLink rl(iface, my_addr);

		if (mode == "send") {
			if (argc < 6)
				throw invalid_argument("send needs <dst> <msg_or_nbytes>");
			uint16_t dst = static_cast<uint16_t>(stoi(argv[4]));

			// If arg is a pure number, treat as byte count; otherwise send as string.
			char* end = nullptr;
			unsigned long nbytes = strtoul(argv[5], &end, 10);
			const bool bulk = (end != argv[5] && *end == '\0' && nbytes > 0);

			if (bulk) {
				vector<char> payload(MAX_PAYLOAD, 'X');
				size_t remaining = nbytes;
				uint32_t pkt_id = 0;
				auto t0 = chrono::high_resolution_clock::now();
				while (remaining > 0) {
					size_t chunk = remaining < MAX_PAYLOAD ? remaining : MAX_PAYLOAD;
					rl.send(dst, ++pkt_id, payload.data(), chunk);
					remaining -= chunk;
				}
				auto t1 = chrono::high_resolution_clock::now();
				double ms = chrono::duration<double, milli>(t1 - t0).count();
				double mbps = (nbytes / (1024.0 * 1024.0)) / (ms / 1000.0);
				cout << "[tx] sent " << nbytes << " bytes in " << pkt_id << " packets, " << ms << " ms, " << mbps
						  << " MiB/s\n";
			} else {
				string msg = argv[5];
				uint32_t pkt_id = 1;
				rl.send(dst, pkt_id, msg.data(), msg.size());
				cout << "[tx] done\n";
			}

		} else if (mode == "recv") {
			rl.recv_loop();

		} else {
			throw invalid_argument("unknown mode: " + mode);
		}

	} catch (const exception& e) {
		cerr << "[error] " << e.what() << '\n';
		return 1;
	}

	return 0;
}
