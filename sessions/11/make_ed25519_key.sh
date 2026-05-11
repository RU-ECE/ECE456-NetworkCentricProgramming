#!/bin/bash
# Generate Ed25519 private key
openssl genpkey -algorithm ed25519 -out key.pem

# Generate self-signed certificate
openssl req -new -x509 -key key.pem -out cert.pem -days 365 -subj "/CN=localhost"
