#include <iostream>
#include <openssl/evp.h>
#include <openssl/pem.h>
#include <string>
#include <vector>

using namespace std;

/**
 * -------------------------
 * Student exercise. Fill in each blank function.
 * Load private key
 * --------------------------
 */
EVP_PKEY* load_private(const char* file) {}

/**
 * -------------------------
 *   Load public key
 * --------------------------
 */
EVP_PKEY* load_public(const char* file) {}


/**
 * -------------------------
 *   SIGN
 * --------------------------
 */
vector<unsigned char> sign_message(EVP_PKEY* priv, const string& msg) { EVP_MD_CTX* ctx = EVP_MD_CTX_new(); }


/**
 * -------------------------
 * VERIFY
 * --------------------------
 */
bool verify_message(EVP_PKEY* pub, const string& msg, const vector<unsigned char>& sig) {
	EVP_MD_CTX* ctx = EVP_MD_CTX_new();
}

int main() {
	EVP_PKEY* priv = load_private("private.pem");
	EVP_PKEY* pub = load_public("public.pem");
	string msg = "Hello, world!";
	vector<unsigned char> sig = sign_message(priv, msg);
	bool verified = verify_message(pub, msg, sig);
	cout << "Verified: " << verified << endl;

	// now make it fail:
	sig[0] = ~sig[0]; // change one byte in the signature

	verified = verify_message(pub, msg, sig);
	cout << "Verified: " << verified << endl;
	return 0;
}
