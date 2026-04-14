/*
 * filequic.cpp — File transfer over QUIC (msquic 2.x)
 *
 * ---- Setup ----
 *
 * Install msquic (Ubuntu 22.04 x64):
 *   wget https://github.com/microsoft/msquic/releases/download/v2.3.5/libmsquic_2.3.5_amd64.deb
 *   wget https://github.com/microsoft/msquic/releases/download/v2.3.5/libmsquic-dev_2.3.5_amd64.deb
 *   sudo dpkg -i libmsquic_2.3.5_amd64.deb libmsquic-dev_2.3.5_amd64.deb
 *
 * Server cert (one time — self-signed is fine for dev):
 *   openssl req -x509 -newkey rsa:2048 -keyout server.key -out server.cert \
 *     -days 365 -nodes -subj '/CN=localhost'
 *
 * ---- Build ----
 *   g++ -std=c++17 -O2 filequic.cpp -o filequic -lmsquic
 *
 * ---- Usage ----
 *   ./filequic recv <outfile> <port>
 *   ./filequic send <infile>  <ip> <port>
 *
 * ---- Notes ----
 *   QUIC mandates TLS 1.3 — there is no plaintext QUIC.
 *   The client skips cert validation (QUIC_CREDENTIAL_FLAG_NO_CERTIFICATE_VALIDATION).
 *   For production: distribute server.cert to clients and validate it.
 *
 *   Protocol (v0 — trivially simple):
 *     One unidirectional QUIC stream carries raw file bytes.
 *     Stream FIN = end of file. Filename is passed on the CLI.
 *     v1 will add a header: [filename_len:2][filename][payload...].
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <cstring>

#include <netinet/in.h>
#include <msquic.h>

/* ------------------------------------------------------------------ */
/* Globals / helpers                                                    */
/* ------------------------------------------------------------------ */

static const QUIC_API_TABLE *MsQuic = nullptr;
static HQUIC                 Reg    = nullptr;

// ALPN token — identifies our protocol to QUIC
static const QUIC_BUFFER Alpn = {
    (uint32_t)(sizeof("filequic") - 1),
    (uint8_t *)"filequic"
};

static void quic_check(QUIC_STATUS st, const char *ctx) {
    if (QUIC_FAILED(st)) {
        std::ostringstream oss;
        oss << ctx << " failed: 0x" << std::hex << st;
        throw std::runtime_error(oss.str());
    }
}

static void quic_init() {
    quic_check(MsQuicOpen2(&MsQuic), "MsQuicOpen2");
    QUIC_REGISTRATION_CONFIG cfg{ "filequic", QUIC_EXECUTION_PROFILE_LOW_LATENCY };
    quic_check(MsQuic->RegistrationOpen(&cfg, &Reg), "RegistrationOpen");
}

static void quic_fini() {
    if (Reg)    MsQuic->RegistrationClose(Reg);
    if (MsQuic) MsQuicClose(MsQuic);
}

// Build a QUIC configuration for server or client.
// Server needs cert+key files; client skips cert validation (dev mode).
static HQUIC make_config(bool is_server,
                         const char *cert_file = nullptr,
                         const char *key_file  = nullptr) {
    QUIC_SETTINGS s{};
    s.IdleTimeoutMs            = 10000;
    s.IsSet.IdleTimeoutMs      = 1;
    if (is_server) {
        // Allow peer (client) to open exactly one unidirectional stream per connection
        s.PeerUnidiStreamCount          = 1;
        s.IsSet.PeerUnidiStreamCount    = 1;
        s.ServerResumptionLevel         = QUIC_SERVER_RESUME_ONLY;
        s.IsSet.ServerResumptionLevel   = 1;
    }

    HQUIC config = nullptr;
    quic_check(MsQuic->ConfigurationOpen(Reg, &Alpn, 1,
                                         &s, sizeof(s),
                                         nullptr, &config),
               "ConfigurationOpen");

    QUIC_CREDENTIAL_CONFIG cred{};
    if (is_server) {
        QUIC_CERTIFICATE_FILE cf{ key_file, cert_file };
        cred.Type            = QUIC_CREDENTIAL_TYPE_CERTIFICATE_FILE;
        cred.CertificateFile = &cf;
        cred.Flags           = QUIC_CREDENTIAL_FLAG_NONE;
        quic_check(MsQuic->ConfigurationLoadCredential(config, &cred),
                   "ConfigurationLoadCredential(server)");
    } else {
        cred.Type  = QUIC_CREDENTIAL_TYPE_NONE;
        // NOTE: disable cert validation for dev/test — remove in production
        cred.Flags = QUIC_CREDENTIAL_FLAG_CLIENT |
                     QUIC_CREDENTIAL_FLAG_NO_CERTIFICATE_VALIDATION;
        quic_check(MsQuic->ConfigurationLoadCredential(config, &cred),
                   "ConfigurationLoadCredential(client)");
    }
    return config;
}

/* ================================================================== */
/* RECEIVER (server)                                                    */
/* ================================================================== */

struct RecvCtx {
    std::string             outfile;
    std::ofstream           ofs;
    uint64_t                bytes_in{0};
    std::atomic<bool>       done{false};
    std::mutex              mtx;
    std::condition_variable cv;
};

static QUIC_STATUS QUIC_API stream_recv_cb(HQUIC Stream, void *ctx, QUIC_STREAM_EVENT *ev) {
    auto *rc = static_cast<RecvCtx *>(ctx);
    switch (ev->Type) {

    case QUIC_STREAM_EVENT_RECEIVE:
        for (uint32_t i = 0; i < ev->RECEIVE.BufferCount; ++i) {
            const auto &b = ev->RECEIVE.Buffers[i];
            rc->ofs.write(reinterpret_cast<const char *>(b.Buffer), b.Length);
            rc->bytes_in += b.Length;
        }
        // Tell msquic we consumed everything
        MsQuic->StreamReceiveComplete(Stream, ev->RECEIVE.TotalBufferLength);
        break;

    case QUIC_STREAM_EVENT_PEER_SEND_SHUTDOWN:
        // Sender closed its end — file is complete
        rc->ofs.close();
        std::cout << "[recv] file complete: " << rc->bytes_in
                  << " bytes -> " << rc->outfile << '\n';
        MsQuic->StreamShutdown(Stream, QUIC_STREAM_SHUTDOWN_FLAG_GRACEFUL, 0);
        break;

    case QUIC_STREAM_EVENT_SHUTDOWN_COMPLETE:
        MsQuic->StreamClose(Stream);
        {
            std::lock_guard<std::mutex> lk(rc->mtx);
            rc->done = true;
        }
        rc->cv.notify_all();
        break;

    default: break;
    }
    return QUIC_STATUS_SUCCESS;
}

static QUIC_STATUS QUIC_API conn_recv_cb(HQUIC Conn, void *ctx, QUIC_CONNECTION_EVENT *ev) {
    auto *rc = static_cast<RecvCtx *>(ctx);
    switch (ev->Type) {

    case QUIC_CONNECTION_EVENT_CONNECTED:
        std::cout << "[recv] client connected\n";
        MsQuic->ConnectionSendResumptionTicket(Conn,
            QUIC_SEND_RESUMPTION_FLAG_NONE, 0, nullptr);
        break;

    case QUIC_CONNECTION_EVENT_PEER_STREAM_STARTED: {
        HQUIC stream = ev->PEER_STREAM_STARTED.Stream;
        rc->ofs.open(rc->outfile, std::ios::binary | std::ios::trunc);
        if (!rc->ofs) {
            std::cerr << "[recv] cannot open output file: " << rc->outfile << '\n';
            MsQuic->ConnectionShutdown(Conn, QUIC_CONNECTION_SHUTDOWN_FLAG_NONE, 1);
            break;
        }
        std::cout << "[recv] stream started, writing to " << rc->outfile << '\n';
        MsQuic->SetCallbackHandler(stream, (void *)stream_recv_cb, ctx);
        break;
    }

    case QUIC_CONNECTION_EVENT_SHUTDOWN_COMPLETE:
        MsQuic->ConnectionClose(Conn);
        break;

    default: break;
    }
    return QUIC_STATUS_SUCCESS;
}

// Listener needs access to both the server Configuration and the RecvCtx
struct ListenerCtx { HQUIC config; RecvCtx *rc; };

static QUIC_STATUS QUIC_API listener_cb(HQUIC, void *ctx, QUIC_LISTENER_EVENT *ev) {
    auto *lc = static_cast<ListenerCtx *>(ctx);
    if (ev->Type == QUIC_LISTENER_EVENT_NEW_CONNECTION) {
        HQUIC conn = ev->NEW_CONNECTION.Connection;
        MsQuic->SetCallbackHandler(conn, (void *)conn_recv_cb, lc->rc);
        if (QUIC_FAILED(MsQuic->ConnectionSetConfiguration(conn, lc->config))) {
            std::cerr << "[recv] ConnectionSetConfiguration failed\n";
            return QUIC_STATUS_CONNECTION_REFUSED;
        }
    }
    return QUIC_STATUS_SUCCESS;
}

static void do_recv(const std::string &outfile, uint16_t port) {
    HQUIC config = make_config(true, "server.cert", "server.key");

    RecvCtx     rc;
    rc.outfile = outfile;
    ListenerCtx lc{ config, &rc };

    HQUIC listener = nullptr;
    quic_check(MsQuic->ListenerOpen(Reg, listener_cb, &lc, &listener),
               "ListenerOpen");

    // Bind to all interfaces on the given port (IPv4 + IPv6 dual stack)
    QUIC_ADDR addr{};
    addr.Ipv4.sin_family = AF_INET;
    addr.Ipv4.sin_port   = htons(port);

    quic_check(MsQuic->ListenerStart(listener, &Alpn, 1, &addr), "ListenerStart");
    std::cout << "[recv] listening on port " << port << '\n';

    // Block main thread until the transfer completes
    std::unique_lock<std::mutex> lk(rc.mtx);
    rc.cv.wait(lk, [&]{ return rc.done.load(); });

    MsQuic->ListenerClose(listener);
    MsQuic->ConfigurationClose(config);
}

/* ================================================================== */
/* SENDER (client)                                                      */
/* ================================================================== */

struct SendCtx {
    std::vector<uint8_t>    data;
    QUIC_BUFFER             buf{};
    HQUIC                   conn{nullptr};
    std::atomic<bool>       done{false};
    std::mutex              mtx;
    std::condition_variable cv;
};

static QUIC_STATUS QUIC_API stream_send_cb(HQUIC Stream, void *ctx, QUIC_STREAM_EVENT *ev) {
    auto *sc = static_cast<SendCtx *>(ctx);
    switch (ev->Type) {

    case QUIC_STREAM_EVENT_SEND_COMPLETE:
        std::cout << "[send] data delivered to QUIC stack\n";
        break;

    case QUIC_STREAM_EVENT_SHUTDOWN_COMPLETE:
        // Stream fully closed — now close the connection
        MsQuic->StreamClose(Stream);
        MsQuic->ConnectionShutdown(sc->conn,
                                   QUIC_CONNECTION_SHUTDOWN_FLAG_NONE, 0);
        break;

    default: break;
    }
    return QUIC_STATUS_SUCCESS;
}

static QUIC_STATUS QUIC_API conn_send_cb(HQUIC Conn, void *ctx, QUIC_CONNECTION_EVENT *ev) {
    auto *sc = static_cast<SendCtx *>(ctx);
    switch (ev->Type) {

    case QUIC_CONNECTION_EVENT_CONNECTED: {
        std::cout << "[send] connected — opening stream\n";
        HQUIC stream = nullptr;
        if (QUIC_FAILED(MsQuic->StreamOpen(Conn,
                                            QUIC_STREAM_OPEN_FLAG_UNIDIRECTIONAL,
                                            stream_send_cb, ctx, &stream))) {
            std::cerr << "[send] StreamOpen failed\n";
            MsQuic->ConnectionShutdown(Conn, QUIC_CONNECTION_SHUTDOWN_FLAG_NONE, 1);
            break;
        }
        if (QUIC_FAILED(MsQuic->StreamStart(stream, QUIC_STREAM_START_FLAG_NONE))) {
            std::cerr << "[send] StreamStart failed\n";
            MsQuic->StreamClose(stream);
            MsQuic->ConnectionShutdown(Conn, QUIC_CONNECTION_SHUTDOWN_FLAG_NONE, 1);
            break;
        }
        // Send the entire file in one shot with FIN — buffer must stay alive
        // until QUIC_STREAM_EVENT_SEND_COMPLETE fires (it points into sc->data)
        sc->buf.Buffer = sc->data.data();
        sc->buf.Length = static_cast<uint32_t>(sc->data.size());
        std::cout << "[send] sending " << sc->data.size() << " bytes\n";
        MsQuic->StreamSend(stream, &sc->buf, 1, QUIC_SEND_FLAG_FIN, nullptr);
        break;
    }

    case QUIC_CONNECTION_EVENT_SHUTDOWN_COMPLETE:
        MsQuic->ConnectionClose(Conn);
        {
            std::lock_guard<std::mutex> lk(sc->mtx);
            sc->done = true;
        }
        sc->cv.notify_all();
        break;

    default: break;
    }
    return QUIC_STATUS_SUCCESS;
}

static void do_send(const std::string &infile,
                    const std::string &host, uint16_t port) {
    // Read the whole file into memory.
    // The QUIC_BUFFER must stay valid until SEND_COMPLETE, so we keep it in SendCtx.
    // TODO: for large files, use a chain of buffers and feed them progressively.
    std::ifstream ifs(infile, std::ios::binary | std::ios::ate);
    if (!ifs) throw std::runtime_error("cannot open: " + infile);
    auto sz = ifs.tellg();
    ifs.seekg(0);

    SendCtx sc;
    sc.data.resize(static_cast<size_t>(sz));
    ifs.read(reinterpret_cast<char *>(sc.data.data()), sz);
    std::cout << "[send] read " << sz << " bytes from " << infile << '\n';

    HQUIC config = make_config(false);

    quic_check(MsQuic->ConnectionOpen(Reg, conn_send_cb, &sc, &sc.conn),
               "ConnectionOpen");
    quic_check(MsQuic->ConnectionStart(sc.conn, config,
                                        QUIC_ADDRESS_FAMILY_UNSPEC,
                                        host.c_str(), port),
               "ConnectionStart");

    std::unique_lock<std::mutex> lk(sc.mtx);
    sc.cv.wait(lk, [&]{ return sc.done.load(); });

    MsQuic->ConfigurationClose(config);
    std::cout << "[send] done\n";
}

/* ================================================================== */
/* main                                                                 */
/* ================================================================== */

int main(int argc, char **argv) {
    try {
        if (argc < 2) {
            std::cerr << "usage:\n"
                      << "  " << argv[0] << " recv <outfile> <port>\n"
                      << "  " << argv[0] << " send <infile> <ip> <port>\n";
            return 1;
        }

        quic_init();

        std::string mode = argv[1];
        if (mode == "recv") {
            if (argc < 4) throw std::invalid_argument("recv needs <outfile> <port>");
            do_recv(argv[2], static_cast<uint16_t>(std::stoi(argv[3])));

        } else if (mode == "send") {
            if (argc < 5) throw std::invalid_argument("send needs <infile> <ip> <port>");
            do_send(argv[2], argv[3], static_cast<uint16_t>(std::stoi(argv[4])));

        } else {
            throw std::invalid_argument("unknown mode: " + mode);
        }

        quic_fini();

    } catch (const std::exception &e) {
        std::cerr << "[error] " << e.what() << '\n';
        return 1;
    }
    return 0;
}