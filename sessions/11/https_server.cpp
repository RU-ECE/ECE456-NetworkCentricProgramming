// minimal_https_server.cpp
#include <arpa/inet.h>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <openssl/err.h>
#include <openssl/ssl.h>
#include <string>
#include <sys/socket.h>
#include <sys/stat.h>
#include <unistd.h>

using namespace std;

static string mime_type(const string& path) {
	if (path.ends_with(".html") || path.ends_with(".htm"))
		return "text/html";
	if (path.ends_with(".txt"))
		return "text/plain";
	if (path.ends_with(".webp"))
		return "image/webp";
	if (path.ends_with(".jpg") || path.ends_with(".jpeg"))
		return "image/jpeg";
	return "text/plain";
}

static bool valid_path(const string& p) { return p.find("..") == string::npos; }

int main(const int argc, char** argv) {
	if (argc != 2) {
		cerr << "usage: " << argv[0] << " <port>" << endl;
		return 1;
	}

	const int port = atoi(argv[1]);

	SSL_library_init();
	SSL_load_error_strings();
	OpenSSL_add_all_algorithms();

	const SSL_METHOD* method = TLS_server_method();
	SSL_CTX* ctx = SSL_CTX_new(method);

	if (!ctx) {
		ERR_print_errors_fp(stderr);
		return 1;
	}

	if (SSL_CTX_use_certificate_file(ctx, "cert.pem", SSL_FILETYPE_PEM) <= 0 ||
		SSL_CTX_use_PrivateKey_file(ctx, "key.pem", SSL_FILETYPE_PEM) <= 0) {
		ERR_print_errors_fp(stderr);
		return 1;
	}

	const int server_fd = socket(AF_INET, SOCK_STREAM, 0);
	constexpr int opt = 1;
	setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

	sockaddr_in addr{};
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(port);

	if (bind(server_fd, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) < 0) {
		perror("bind");
		return 1;
	}

	if (listen(server_fd, 16) < 0) {
		perror("listen");
		return 1;
	}

	cout << "HTTPS server on https://localhost:" << port << endl;

	while (true) {
		sockaddr_in client{};
		socklen_t len = sizeof(client);

		const int cfd = accept(server_fd, reinterpret_cast<sockaddr*>(&client), &len);
		if (cfd < 0)
			continue;

		SSL* ssl = SSL_new(ctx);
		SSL_set_fd(ssl, cfd);

		if (SSL_accept(ssl) <= 0) {
			SSL_free(ssl);
			close(cfd);
			continue;
		}

		char buf[4096];
		const int n = SSL_read(ssl, buf, sizeof(buf) - 1);
		if (n <= 0) {
			SSL_shutdown(ssl);
			SSL_free(ssl);
			close(cfd);
			continue;
		}
		buf[n] = '\0';

		{
			string req(buf);

			const size_t m0 = req.find("GET ");
			const size_t m1 = req.find(" HTTP/");
			if (m0 == string::npos || m1 == string::npos) {
				const auto resp = "HTTP/1.1 400 Bad Request\r\nContent-Length: 0\r\n\r\n";
				SSL_write(ssl, resp, strlen(resp));
				goto cleanup;
			}

			string path = req.substr(m0 + 4, m1 - (m0 + 4));
			if (path == "/")
				path = "/index.html";

			if (!valid_path(path)) {
				const auto resp = "HTTP/1.1 403 Forbidden\r\nContent-Length: 0\r\n\r\n";
				SSL_write(ssl, resp, strlen(resp));
				goto cleanup;
			}

			string full = "./www" + path;
			const int fd = open(full.c_str(), O_RDONLY);

			if (fd < 0) {
				const auto body = "404 Not Found";
				string hdr = "HTTP/1.1 404 Not Found\r\n"
							 "Content-Type: text/plain\r\n"
							 "Content-Length: " +
					to_string(strlen(body)) + "\r\n\r\n";
				SSL_write(ssl, hdr.c_str(), hdr.size());
				SSL_write(ssl, body, strlen(body));
				goto cleanup;
			}

			struct stat st {};
			fstat(fd, &st);

			string hdr = "HTTP/1.1 200 OK\r\n"
						 "Content-Type: " +
				mime_type(full) +
				"\r\n"
				"Content-Length: " +
				to_string(st.st_size) + "\r\n\r\n";

			SSL_write(ssl, hdr.c_str(), hdr.size());

			ssize_t r;
			while ((r = read(fd, buf, sizeof(buf))) > 0)
				SSL_write(ssl, buf, r);

			close(fd);
		}

	cleanup:
		SSL_shutdown(ssl);
		SSL_free(ssl);
		close(cfd);
	}

	close(server_fd);
	SSL_CTX_free(ctx);
	return 0;
}
