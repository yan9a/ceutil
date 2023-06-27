// Description: Simple functions to calculate DES (Data Encryption Standard) and 3DES (Triple DES)
// Class implementation of the following repository: https://github.com/yan9a/ceDES
// WebSite: http://cool-emerald.blogspot.com
// MIT License (https://opensource.org/licenses/MIT)
// Copyright (c) 2023 Yan Naing Aye

// Related links
// https://www.rapidtables.com/convert/number/ascii-to-hex.html
// https://emvlab.org/descalc/
// https://paymentcardtools.com/crypto-calculators/des-calculator
// https://github.com/yan9a/cecpp/blob/master/des_cryptopp/main.cpp

#ifndef CEDES_H
#define CEDES_H

#include<string>

namespace ce {
	class ceDES {
	private:
        //-----------------------------------------------------------------------------
        // Init permutation table 
        // 64 bit data -> 64 bit data
        static const unsigned char IP_table[64];
        //-----------------------------------------------------------------------------
        //Permuted Choice 1
        // Key permutation table
        // 64 bit key data -> 56 bit key data
        static const unsigned char PC1_table[56];
        //-----------------------------------------------------------------------------
        // Key shift table
        static const unsigned char LeftShift_table[16];
        //-----------------------------------------------------------------------------
        // Key Compression permutation table
        // 56 bit key data -> 48 bit 
        static const unsigned char PC2_table[48];
        //-----------------------------------------------------------------------------
        // Expansion permutation table
        // 32 bit data -> 48 bit data
        static const unsigned char E_table[48];
        //-----------------------------------------------------------------------------
        static const unsigned char S[8][4][16];
        //-----------------------------------------------------------------------------
        // Strait permutation table
        // 32 bit data -> 32 bit data
        static const unsigned char P_table[32];
        //-----------------------------------------------------------------------------
        // Inverse permutation table 
        // 64 bit data -> 64 bit data
        static const unsigned char InverseIP_table[64];
		//-----------------------------------------------------------------------------
        static unsigned char Kn[7];
        static unsigned char Rn_1[6];
        //-----------------------------------------------------------------------------
        static void Permutate(unsigned char* pData, unsigned char ni,
            unsigned char* pTable, unsigned char no);
        static void LeftShift(unsigned char* nKey, unsigned char b);
        static void RightShift(unsigned char* nKey, unsigned char b);
        static void Expand(unsigned char* R);
        static void F(unsigned char* R, unsigned char* K);
		//-----------------------------------------------------------------------------
        static void Encrypt(unsigned char* dMes, unsigned char* dKey); // DES encryption
        static void Decrypt(unsigned char* dMes, unsigned char* dKey); // DES decryption
        static void Encrypt(unsigned char* dMes, unsigned char* dKeyL, unsigned char* dKeyR);// TDES encryption
        static void Decrypt(unsigned char* dMes, unsigned char* dKeyL, unsigned char* dKeyR);// TDES decryption
	public:
        static std::string Encrypt(std::string dMes, std::string dKey); // DES encryption, DES if dKey is 8 bytes, and TDES if it is 16 bytes
        static std::string Decrypt(std::string dMes, std::string dKey); // DES decryption, DES if dKey is 8 bytes, and TDES if it is 16 bytes
	};
} // namespace ce 
#endif // CEDES_H