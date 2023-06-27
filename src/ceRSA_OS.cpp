// Description: Functions to calculate SHA1
// Class implementation of the following repository: https://github.com/clibs/sha1

#include "ce/ceMacros.h"
#include "ce/ceRSA_OS.h"

#ifdef CE_WINDOWS
#ifdef _MSC_VER
#pragma warning(disable : 4996)
#endif
#endif

using namespace std;
namespace ce {
// ----------------------------------------------------------------------------

// Load public key from file
RSA* ceRSA_OS::loadPublicKey(std::string publicKeyFile) {
    RSA* keypair = RSA_new();
    FILE* publicKey = fopen(publicKeyFile.c_str(), "r");
    PEM_read_RSAPublicKey(publicKey, &keypair, NULL, NULL);
    fclose(publicKey);
    return keypair;
}

// Load private key from file
RSA* ceRSA_OS::loadPrivateKey(std::string privateKeyFile) {
    RSA* keypair = RSA_new();
    FILE* bigN = fopen(privateKeyFile.c_str(), "r");
    PEM_read_RSAPrivateKey(bigN, &keypair, NULL, NULL);
    fclose(bigN);
    return keypair;
}

// Encrypt data using public key
std::string ceRSA_OS::encrypt(const std::string& message, RSA* rsa_key) {
    int rsa_size = RSA_size(rsa_key);
    std::vector<unsigned char> ciphertext(rsa_size);

    int len = RSA_public_encrypt(message.size(), reinterpret_cast<const unsigned char*>(message.data()),
        ciphertext.data(), rsa_key, RSA_NO_PADDING);
    //ciphertext.data(), rsa_key, RSA_PKCS1_PADDING);

    if (len < 0) {
        std::cerr << "Encryption failed" << std::endl;
        return "";
    }

    return std::string(reinterpret_cast<const char*>(ciphertext.data()), len);
}

// Decrypt data using private key
std::string ceRSA_OS::decrypt(const std::string& ciphertext, RSA* rsa_key) {
    int rsa_size = RSA_size(rsa_key);
    std::vector<unsigned char> message(rsa_size);

    int len = RSA_private_decrypt(ciphertext.size(), reinterpret_cast<const unsigned char*>(ciphertext.data()),
        message.data(), rsa_key, RSA_NO_PADDING);

    if (len < 0) {
        std::cerr << "Decryption failed" << std::endl;
        return "";
    }

    return std::string(reinterpret_cast<const char*>(message.data()), len);
}

string ceRSA_OS::bn2str(BIGNUM* bigN)
{
    // Convert BIGNUM to binary representation
    size_t bnSize = BN_num_bytes(bigN);
    unsigned char* bnStr = new unsigned char[bnSize];
    BN_bn2bin(bigN, bnStr);
    string bnString((char*)bnStr, bnSize);
    return bnString;
}

// ----------------------------------------------------------------------------

// Generate a public-private key pair and save to files
RSA* ceRSA_OS::generateKeyPair(std::string publicKeyFile, std::string privateKeyFile, int keyLength, int exp) {
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

// Encrypt data using public key
std::string ceRSA_OS::encrypt(const std::string& message, std::string publicKeyFile)
{
    // Load public key
    RSA* public_key = loadPublicKey(publicKeyFile);
    std::string encrypted = encrypt(message, public_key);
    return encrypted;
}

// Decrypt data using private key
std::string ceRSA_OS::decrypt(const std::string& ciphertext, std::string privateKeyFile)
{
    // Load private key
    RSA* private_key = loadPrivateKey(privateKeyFile);
    std::string decrypted = decrypt(ciphertext, private_key);
    return decrypted;
}

// Encrypt data using modulus n and exponent e
std::string ceRSA_OS::encrypt(const std::string& message, std::string n, std::string e)
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
std::string ceRSA_OS::decrypt(const std::string& ciphertext, std::string d, std::string n, std::string e)
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

// ----------------------------------------------------------------------------
} // namespace ce 