#include <iostream>
#include <openssl/evp.h> // For EVP_* (modern OpenSSL API)
#include <openssl/rand.h> // For RAND_bytes
#include <string>

using namespace std;

int main(int argc, char** argv) {
	if (argc < 3) { // Fixed: you need at least 2 args (key + filename)
		cerr << "Usage: " << argv[0] << " <keyfile> <filename>" << endl;
		return 1;
	}

	const char* keyfilename = argv[1];
	const char* filename = argv[2]; // Fixed: was argv[2] but argc check only required 1

	// Correct OpenSSL types
	unsigned char key[32]; // 256-bit key
	unsigned char iv[16]; // 128-bit IV for AES

	// Load key from file (manual file read, OpenSSL has no load_key helper)
	FILE* kf = fopen(keyfilename, "rb");
	if (!kf) {
		cerr << "Cannot open key file" << endl;
		return 1;
	}
	fread(key, 1, sizeof(key), kf);
	fclose(kf);

	// Open input and output files
	FILE* fin = fopen(filename, "rb");
	FILE* fout = fopen((string(filename) + ".enc").c_str(), "wb");
	EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
	if (!fin || !fout) {
		cerr << "Cannot open file(s)" << endl;
		EVP_CIPHER_CTX_free(ctx);
		return 1;
	}

	EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, key, iv);

	const int BLOCK_SIZE = 4096;
	unsigned char inbuf[BLOCK_SIZE];
	unsigned char outbuf[BLOCK_SIZE + EVP_MAX_BLOCK_LENGTH]; // extra space for padding
	int outlen = 0;
	int bytes_read = 0;

	// Read and encrypt chunks until EOF
	while ((bytes_read = fread(inbuf, 1, BLOCK_SIZE, fin)) > 0) {
		if (!EVP_EncryptUpdate(ctx, outbuf, &outlen, inbuf, bytes_read)) {
			cerr << "EVP_EncryptUpdate failed" << endl;
			EVP_CIPHER_CTX_free(ctx);
			fclose(fin);
			fclose(fout);
			return 1;
		}
		fwrite(outbuf, 1, outlen, fout);
	}

	// Flush the final partial block (adds PKCS#7 padding)
	if (!EVP_EncryptFinal_ex(ctx, outbuf, &outlen)) {
		cerr << "EVP_EncryptFinal_ex failed" << endl;
		EVP_CIPHER_CTX_free(ctx);
		fclose(fin);
		fclose(fout);
		return 1;
	}
	fwrite(outbuf, 1, outlen, fout);

	EVP_CIPHER_CTX_free(ctx);
	fclose(fin);
	fclose(fout);

	return 0;
}
