/////////////////////////////////////////////////////////////////////////////
// Name:         cersaos.h
// Description:  Functions to calculate RSA using OpenSSL crypto lib
// Author:       Yan Naing Aye
/////////////////////////////////////////////////////////////////////////////
 // Class implementation of the following example: https://github.com/yan9a/cecpp/blob/master/rsa_openssl/main.cpp
#ifndef CERSA_OS_H
#define CERSA_OS_H

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

#ifdef CE_WINDOWS
#ifdef _MSC_VER
#pragma warning(disable : 4996)
#endif
#endif

// ----------------------------------------------------------------------------

// Load public key from file
inline RSA* ceRSA_OS::loadPublicKey(std::string publicKeyFile) {
    RSA* keypair = RSA_new();
    FILE* publicKey = fopen(publicKeyFile.c_str(), "r");
    PEM_read_RSAPublicKey(publicKey, &keypair, NULL, NULL);
    fclose(publicKey);
    return keypair;
}

// Load private key from file
inline RSA* ceRSA_OS::loadPrivateKey(std::string privateKeyFile) {
    RSA* keypair = RSA_new();
    FILE* bigN = fopen(privateKeyFile.c_str(), "r");
    PEM_read_RSAPrivateKey(bigN, &keypair, NULL, NULL);
    fclose(bigN);
    return keypair;
}

// Encrypt data using public key
inline std::string ceRSA_OS::encrypt(const std::string& message, RSA* rsa_key) {
    int rsa_size = RSA_size(rsa_key);
    std::vector<unsigned char> ciphertext(rsa_size);

    int len = RSA_public_encrypt(message.size(), reinterpret_cast<const unsigned char*>(message.data()),
        ciphertext.data(), rsa_key, RSA_NO_PADDING);
    //ciphertext.data(), rsa_key, RSA_PKCS1_PADDING);

    if (len < 0) {
        // std::cerr << "Encryption failed" << std::endl;
        return "";
    }

    return std::string(reinterpret_cast<const char*>(ciphertext.data()), len);
}

// Decrypt data using private key
inline std::string ceRSA_OS::decrypt(const std::string& ciphertext, RSA* rsa_key) {
    int rsa_size = RSA_size(rsa_key);
    std::vector<unsigned char> message(rsa_size);

    int len = RSA_private_decrypt(ciphertext.size(), reinterpret_cast<const unsigned char*>(ciphertext.data()),
        message.data(), rsa_key, RSA_NO_PADDING);

    if (len < 0) {
        // std::cerr << "Decryption failed" << std::endl;
        return "";
    }

    return std::string(reinterpret_cast<const char*>(message.data()), len);
}

inline std::string ceRSA_OS::bn2str(BIGNUM* bigN)
{
    // Convert BIGNUM to binary representation
    size_t bnSize = BN_num_bytes(bigN);
    unsigned char* bnStr = new unsigned char[bnSize];
    BN_bn2bin(bigN, bnStr);
    std::string bnString((char*)bnStr, bnSize);
    return bnString;
}

// ----------------------------------------------------------------------------

// Generate a public-private key pair and save to files
inline RSA* ceRSA_OS::generateKeyPair(std::string publicKeyFile, std::string privateKeyFile, uint32_t keyLength, uint32_t exp) {
    RSA* keypair = RSA_new();
    BIGNUM* e = BN_new();
    BN_set_word(e, exp);

    RSA_generate_key_ex(keypair, keyLength, e, NULL);
    FILE* publicKey = fopen(publicKeyFile.c_str(), "w");
    PEM_write_RSAPublicKey(publicKey, keypair);
    fclose(publicKey);

    FILE* bigN = fopen(privateKeyFile.c_str(), "w");
    PEM_write_RSAPrivateKey(bigN, keypair, NULL, NULL, 0, NULL, NULL);
    fclose(bigN);

    BN_free(e);
    return keypair;
}

// Generate a public-private key pair and returns RSA
inline RSA* ceRSA_OS::generateKeyPair(uint32_t keyLength, uint32_t exp) {
    RSA* keypair = RSA_new();
    BIGNUM* e = BN_new();
    BN_set_word(e, exp);
    RSA_generate_key_ex(keypair, keyLength, e, NULL);
    BN_free(e);
    return keypair;
}

// Encrypt data using public key
inline std::string ceRSA_OS::encrypt(const std::string& message, std::string publicKeyFile)
{
    // Load public key
    RSA* public_key = loadPublicKey(publicKeyFile);
    std::string encrypted = encrypt(message, public_key);
    return encrypted;
}

// Decrypt data using private key
inline std::string ceRSA_OS::decrypt(const std::string& ciphertext, std::string privateKeyFile)
{
    // Load private key
    RSA* private_key = loadPrivateKey(privateKeyFile);
    std::string decrypted = decrypt(ciphertext, private_key);
    return decrypted;
}

// Encrypt data using modulus n and exponent e
inline std::string ceRSA_OS::encrypt(const std::string& message, std::string n, std::string e)
{
    BIGNUM* bn_e3 = BN_new();
    BIGNUM* bn_n3 = BN_new();

    if (BN_bin2bn((const unsigned char*)e.c_str(), e.length(), bn_e3) == NULL) {
        printf("Failed to convert modulus");
    }
    if (BN_bin2bn((const unsigned char*)n.c_str(), n.length(), bn_n3) == NULL) {
        printf("Failed to convert exponent");
    }

    RSA* public_key3 = RSA_new();
    RSA_set0_key(public_key3, bn_n3, bn_e3, NULL);

    std::string encrypted = encrypt(message, public_key3);
    return encrypted;
}

// Decrypt data using secret number d, modulus n and exponent e
inline std::string ceRSA_OS::decrypt(const std::string& ciphertext, std::string d, std::string n, std::string e)
{
    BIGNUM* bn_d3 = BN_new();
    BIGNUM* bn_e3 = BN_new();
    BIGNUM* bn_n3 = BN_new();
    if (BN_bin2bn((const unsigned char*)d.c_str(), d.length(), bn_d3) == NULL) {
        printf("Failed to convert d");
    }
    if (BN_bin2bn((const unsigned char*)e.c_str(), e.length(), bn_e3) == NULL) {
        printf("Failed to convert modulus");
    }
    if (BN_bin2bn((const unsigned char*)n.c_str(), n.length(), bn_n3) == NULL) {
        printf("Failed to convert exponent");
    }

    RSA* private_key3 = RSA_new();
    RSA_set0_key(private_key3, bn_n3, bn_e3, bn_d3);

    std::string decrypted = decrypt(ciphertext, private_key3);
    return decrypted;
}

// Get big numbers as strings for secret number d, modulus n and exponent e as reference from RSA key
inline void ceRSA_OS::getBN(RSA* key, std::string& d, std::string& n, std::string& e)
{
    // Obtain the key components as BIGNUM
    BIGNUM* bn_d = BN_dup(RSA_get0_d(key));
    BIGNUM* bn_e = BN_dup(RSA_get0_e(key));
    BIGNUM* bn_n = BN_dup(RSA_get0_n(key));

    d = bn2str(bn_d);
    e = bn2str(bn_e);
    n = bn2str(bn_n);
}

// ----------------------------------------------------------------------------
} // namespace ce 
#endif // CERSA_OS_H
