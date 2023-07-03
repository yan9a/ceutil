#ifndef CERSA_OS_H
#define CERSA_OS_H

 // Description: Functions to calculate RSA using OpenSSL crypto lib
 // Class implementation of the following example: https://github.com/yan9a/cecpp/blob/master/rsa_openssl/main.cpp

#include <cstdint>
#include <iostream>
#include <bitset>
#include <string>
#include <sstream>
#include <fstream>
#include <cstring>
#include <iomanip>
#include <vector>
#include <openssl/bio.h>
#include <openssl/pem.h>
#include <openssl/rsa.h>
#include <openssl/bn.h>
#include <algorithm>
namespace ce {
    class ceRSA_OS {
    public:
        // ----------------------------------------------------------------------------
        static std::string bn2str(BIGNUM* bigN);

        // Load public key from file
        static RSA* loadPublicKey(std::string publicKeyFile);

        // Load private key from file
        static RSA* loadPrivateKey(std::string privateKeyFile);

        // Encrypt data using public key
        static std::string encrypt(const std::string& message, RSA* rsa_key);

        // Decrypt data using private key
        static std::string decrypt(const std::string& ciphertext, RSA* rsa_key);

        // Generate a public-private key pair and save to files
        static RSA* generateKeyPair(std::string publicKeyFile, std::string privateKeyFile, uint32_t bit_len = 2048, uint32_t exp = 3);

        // Generate a public-private key pair and returns RSA
        static RSA* generateKeyPair(uint32_t bit_len = 2048, uint32_t exp = 3);

        // Encrypt data using public key
        static std::string encrypt(const std::string& message, std::string publicKeyFile);

        // Decrypt data using private key
        static std::string decrypt(const std::string& ciphertext, std::string privateKeyFile);

        // Encrypt data using modulus n and exponent e
        static std::string encrypt(const std::string& message, std::string n, std::string e);

        // Decrypt data using secret number d, modulus n and exponent e
        static std::string decrypt(const std::string& ciphertext, std::string d, std::string n, std::string e);

        // Get big numbers as strings for secret number d, modulus n and exponent e as reference from RSA key
        static void getBN(RSA* key, std::string& d, std::string& n, std::string& e);
    };
} // namespace ce 

#endif // CERSA_OS_H
