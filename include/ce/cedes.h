/////////////////////////////////////////////////////////////////////////////
// File: cedes.h
// Description: Simple C functions to calculate DES (Data Encryption Standard) and 3DES (Triple DES)
// Repository: https://github.com/yan9a/cedes
// WebSite: http://cool-emerald.blogspot.com
// MIT License (https://opensource.org/licenses/MIT)
// Copyright (c) 2023 Yan Naing Aye
/////////////////////////////////////////////////////////////////////////////
// Related links
// https://www.rapidtables.com/convert/number/ascii-to-hex.html
// https://emvlab.org/descalc/
// https://paymentcardtools.com/crypto-calculators/des-calculator
// https://github.com/yan9a/cecpp/blob/master/des_cryptopp/main.cpp
/////////////////////////////////////////////////////////////////////////////

#ifndef CEDES_H
#define CEDES_H

#if defined(__cplusplus)
extern "C" {
#endif

// Functions
// void ceDES_Encrypt(unsigned char *dMes,unsigned char *dKey);
// void ceDES_Decrypt(unsigned char *dMes,unsigned char *dKey);
// void ceTDES_Encrypt(unsigned char *dMes,unsigned char *dKeyL,unsigned char *dKeyR);
// void ceTDES_Decrypt(unsigned char *dMes,unsigned char *dKeyL,unsigned char *dKeyR);

/////////////////////////////////////////////////////////////////////////////
// Implementation

//-----------------------------------------------------------------------------
// Init permutation table 
// 64 bit data -> 64 bit data
const unsigned char IP_table[64] = {          
                   58, 50, 42, 34, 26, 18, 10, 2,          
                   60, 52, 44, 36, 28, 20, 12, 4,
                   62, 54, 46, 38, 30, 22, 14, 6,
                   64, 56, 48, 40, 32, 24, 16, 8,
                   57, 49, 41, 33, 25, 17,  9, 1,
                   59, 51, 43, 35, 27, 19, 11, 3,
                   61, 53, 45, 37, 29, 21, 13, 5,
                   63, 55, 47, 39, 31, 23, 15, 7 };
//-----------------------------------------------------------------------------
//Permuted Choice 1
// Key permutation table
// 64 bit key data -> 56 bit key data
const unsigned char PC1_table[56] = {           
                   57, 49, 41, 33, 25, 17,  9,             
                    1, 58, 50, 42, 34, 26, 18,
                   10,  2, 59, 51, 43, 35, 27,
                   19, 11,  3, 60, 52, 44, 36,
                   63, 55, 47, 39, 31, 23, 15,
                    7, 62, 54, 46, 38, 30, 22,
                   14,  6, 61, 53, 45, 37, 29,
                   21, 13,  5, 28, 20, 12,  4 };
//-----------------------------------------------------------------------------
// Key shift table
const unsigned char LeftShift_table[16] = {                 
                    1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1 };
//-----------------------------------------------------------------------------
// Key Compression permutation table
// 56 bit key data -> 48 bit 
const unsigned char PC2_table[48] = {   
                   14, 17, 11, 24,  1,  5,  3, 28,         
                   15,  6, 21, 10, 23, 19, 12,  4,
                   26,  8, 16,  7, 27, 20, 13,  2,
                   41, 52, 31, 37, 47, 55, 30, 40,
                   51, 45, 33, 48, 44, 49, 39, 56,
                   34, 53, 46, 42, 50, 36, 29, 32 }; 
//-----------------------------------------------------------------------------
// Expansion permutation table
// 32 bit data -> 48 bit data
const unsigned char E_table[48] = {     
                   32,  1,  2,  3,  4,  5,                 
                    4,  5,  6,  7,  8,  9,
                    8,  9, 10, 11, 12, 13,
                   12, 13, 14, 15, 16, 17,
                   16, 17, 18, 19, 20, 21,
                   20, 21, 22, 23, 24, 25,
                   24, 25, 26, 27, 28, 29,
                   28, 29, 30, 31, 32,  1 };
//-----------------------------------------------------------------------------
const unsigned char S[8][4][16] = { 
        {
        {14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7}, 
        {0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8}, 
        {4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0}, 
        {15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13}
        }, 
        {
        {15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10}, 
        {3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5}, 
        {0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15}, 
        {13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9}
        }, 
        {
        {10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8}, 
        {13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1}, 
        {13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7}, 
        {1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12}
        }, 
        {
        {7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15}, 
        {13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9}, 
        {10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4}, 
        {3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14}
        },
        {
        {2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9}, 
        {14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6}, 
        {4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14}, 
        {11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3}
        }, 
        {
        {12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11}, 
        {10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8}, 
        {9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6}, 
        {4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13}
        }, 
        {
        {4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1}, 
        {13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6}, 
        {1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2}, 
        {6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12}
        }, 
        {
        {13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7}, 
        {1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2}, 
        {7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8}, 
        {2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11}
        }
    };
//-----------------------------------------------------------------------------
// Strait permutation table
// 32 bit data -> 32 bit data
const unsigned char P_table[32] = {        
                   16,  7, 20, 21, 29, 12, 28, 17,         
                    1, 15, 23, 26,  5, 18, 31, 10,
                    2,  8, 24, 14, 32, 27,  3,  9,
                   19, 13, 30,  6, 22, 11,  4, 25 };
//-----------------------------------------------------------------------------
// Inverse permutation table 
// 64 bit data -> 64 bit data
const unsigned char InverseIP_table[64] = {      
                   40, 8, 48, 16, 56, 24, 64, 32,          
                   39, 7, 47, 15, 55, 23, 63, 31,
                   38, 6, 46, 14, 54, 22, 62, 30,
                   37, 5, 45, 13, 53, 21, 61, 29,
                   36, 4, 44, 12, 52, 20, 60, 28,
                   35, 3, 43, 11, 51, 19, 59, 27,
                   34, 2, 42, 10, 50, 18, 58, 26,
                   33, 1, 41,  9, 49, 17, 57, 25 };
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//pData  =input to permutate
//ni     =number of input bytes
//pTable =permutation table
//no     =number of output bytes
void ceDES_Permutate(unsigned char *pData,unsigned char ni,
                   unsigned char *pTable,unsigned char no)
{
	unsigned char temp[8];
	unsigned char i,j,x;
	unsigned char ByteI,BitIndex;
	
	for(i=0;i<ni;i++) temp[i]=pData[i];
	
	for(i=0;i<no;i++,pData++)
	{ 
		x=0;
		for(j=0;j<8;j++,pTable++)
		{
			x<<=1;
			BitIndex=*pTable-1;//index starts from 0 whereas table index starts from 1
			ByteI=temp[BitIndex>>3];//divided by 8
			BitIndex =7-(BitIndex & 7);//mod 8
			x|=((ByteI>>BitIndex) & 0x01);								
		}
		*pData=x;
	}
	
};
//-----------------------------------------------------------------------------
//nKey -Key to shift
//b -number of bits
//priority to speed rather than code size
void ceDES_LeftShift(unsigned char *nKey,unsigned char b)
{
	unsigned char x,y,c;	
	c=8-b;
	
	//avoid loop to get linear code
	x=nKey[6];
	nKey[6]=x<<b;
	y=x>>c;
	
	x=nKey[5];	
	nKey[5]=(x<<b)|y;
	y=x>>c;
	
	x=nKey[4];	
	nKey[4]=(x<<b)|y;
	y=x>>c;
	
	x=nKey[3];	
	nKey[3]=(x<<b)|y;
	y=x>>c;
	
	x<<=4;
	nKey[6]|=x>>c;
	
	x=nKey[2];	
	nKey[2]=(x<<b)|y;
	y=x>>c;
	
	x=nKey[1];	
	nKey[1]=(x<<b)|y;
	y=x>>c;
	
	x=nKey[0];	
	nKey[0]=(x<<b)|y;
	y=x>>c;
	
	x=(b==1)?0xEF:0xCF;
	x &=nKey[3];
	nKey[3]=x|(y<<4);
};
//-----------------------------------------------------------------------------
//nKey -Key to shift
//b -number of bits
//written for speed
void ceDES_RightShift(unsigned char *nKey,unsigned char b)
{
	unsigned char x,y,c;	
	c=8-b;
	
	//avoid loop to get linear code
	x=nKey[0];
	nKey[0]=x>>b;
	y=x<<c;
	
	x=nKey[1];	
	nKey[1]=(x>>b)|y;
	y=x<<c;
	
	x=nKey[2];	
	nKey[2]=(x>>b)|y;
	y=x<<c;
	
	x=nKey[3];	
	nKey[3]=(x>>b)|y;
	y=x<<c;
	
	x>>=4;
	nKey[0]|=x<<c;
	
	x=nKey[4];	
	nKey[4]=(x>>b)|y;
	y=x<<c;
	
	x=nKey[5];	
	nKey[5]=(x>>b)|y;
	y=x<<c;
	
	x=nKey[6];	
	nKey[6]=(x>>b)|y;
	y=x<<c;
	
	x=(b==1)?0xF7:0xF3;
	x &=nKey[3];
	nKey[3]=x|(y>>4);
};
//-----------------------------------------------------------------------------
//written for speed
void ceDES_Expand(unsigned char *R)
{
	unsigned char a,b,c,d,x;
	a=R[0];
	b=R[1];
	c=R[2];
	d=R[3];
	
	x=(d<<7) & 0x80;//bit 32 to 1	
	x|=((a>>1) & 0x7C);
	x|=((a>>3) & 0x03);
	R[0]=x;
	
	x=(a<<5) & 0xE0;
	x|=(b>>3) & 0x10;
	x|=(a<<3) & 0x08;
	x|=(b>>5) & 0x07;
	R[1]=x;
	
	x=(b<<3) & 0xC0;
	x|=((b<<1) & 0x3E);
	x|=((c>>7) & 0x01);
	R[2]=x;
	
	x=(b<<7) & 0x80;
	x|=((c>>1) & 0x7C);
	x|=((c>>3) & 0x03);
	R[3]=x;
	
	x=(c<<5) & 0xE0;
	x|=(d>>3) & 0x10;
	x|=(c<<3) & 0x08;
	x|=(d>>5) & 0x07;
	R[4]=x;
	
	x=(d<<3) & 0xC0;
	x|=((d<<1) & 0x3E);
	x|=((a>>7) & 0x01);
	R[5]=x;
};
//-----------------------------------------------------------------------------
//written for speed
void ceDES_F(unsigned char *R,unsigned char *K)
{
	unsigned char i,r,c;
	
	//special function for expansion to improve speed
	ceDES_Expand(R);	
	
	for(i=0;i<6;i++) K[i]^=R[i];
	
	//R[0]=S0 S1
	c=(K[0]>>3) & 0x0F;
	r=(K[0]>>2) & 1;
	r|=(K[0]>>6) & 2;	
	R[0]=(S[0][r][c])<<4;
	
	c=(K[0]<<3) & 0x08;
	c |=(K[1]>>5) & 0x07;	
	r=(K[1]>>4) & 1;
	r|=(K[0] & 2);
	R[0]|=(S[1][r][c]);
	
	//R[1]=S2 S3
	c=(K[1]<<1) & 0x0E;
	c|=(K[2]>>7) & 1;
	r=(K[2]>>6) & 1;
	r|=(K[1]>>2) & 2;	
	R[1]=(S[2][r][c])<<4;
	
	c=(K[2]>>1) & 0x0F;		
	r=K[2] & 1;
	r|=(K[2]>>4) & 2;
	R[1]|=(S[3][r][c]);
	
	//R[2]=S4 S5
	c=(K[3]>>3) & 0x0F;
	r=(K[3]>>2) & 1;
	r|=(K[3]>>6) & 2;	
	R[2]=(S[4][r][c])<<4;
	
	c=(K[3]<<3) & 0x08;
	c |=(K[4]>>5) & 0x07;	
	r=(K[4]>>4) & 1;
	r|=(K[3] & 2);
	R[2]|=(S[5][r][c]);
	
	//R[3]=S6 S7
	c=(K[4]<<1) & 0x0E;
	c|=(K[5]>>7) & 1;
	r=(K[5]>>6) & 1;
	r|=(K[4]>>2) & 2;	
	R[3]=(S[6][r][c])<<4;
	
	c=(K[5]>>1) & 0x0F;		
	r=K[5] & 1;
	r|=(K[5]>>4) & 2;
	R[3]|=(S[7][r][c]);
	
	//Permutate the S box output
	ceDES_Permutate(R,4,(unsigned char *)P_table,4);//32 bit output
};
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//dMes=Message to encrypt
//dKey=Key
void ceDES_Encrypt(unsigned char *dMes,unsigned char *dKey)
{
	unsigned char Kn[7];
	unsigned char Rn_1[6];
	unsigned char n,i;	
	unsigned char dKeyC[8];
	for(i=0;i<8;i++) dKeyC[i]=dKey[i];//copy key
	
	ceDES_Permutate(dMes,8,(unsigned char *)IP_table,8);//64 bit output
	ceDES_Permutate(dKeyC,8,(unsigned char *)PC1_table,7);//56 bit output	
	for(n=0;n<16;n++)
	{
		ceDES_LeftShift(dKeyC,LeftShift_table[n]);
		for(i=0;i<7;i++) Kn[i]=dKeyC[i];
		ceDES_Permutate(Kn,7,(unsigned char *)PC2_table,6);//48 bit output			
		for(i=0;i<4;i++) Rn_1[i]=dMes[i+4];	
		ceDES_F(Rn_1,Kn);	
		
		if(n>=15)
		{
			for(i=0;i<4;i++) dMes[i]^=Rn_1[i];
			break;
		}
		for(i=0;i<4;i++) Rn_1[i]^=dMes[i];		
		for(i=0;i<4;i++) dMes[i]=dMes[i+4];
		for(i=0;i<4;i++) dMes[i+4]=Rn_1[i];			
	}	
	ceDES_Permutate(dMes,8,(unsigned char *)InverseIP_table,8);//64 bit output
};
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//dMes=Message to decrypt
//dKey=Key
void ceDES_Decrypt(unsigned char *dMes,unsigned char *dKey)
{
	unsigned char Kn[7];
	unsigned char Rn_1[6];
	unsigned char n,i;	
	unsigned char dKeyC[8];
	for(i=0;i<8;i++) dKeyC[i]=dKey[i];//copy key
	
	ceDES_Permutate(dMes,8,(unsigned char *)IP_table,8);//64 bit output
	ceDES_Permutate(dKeyC,8,(unsigned char *)PC1_table,7);//56 bit output
	
	for(n=0;n<16;n++)
	{		
		for(i=0;i<7;i++) Kn[i]=dKeyC[i];
		ceDES_Permutate(Kn,7,(unsigned char *)PC2_table,6);//48 bit output			
		for(i=0;i<4;i++) Rn_1[i]=dMes[i+4];			
		ceDES_F(Rn_1,Kn);	
		
		if(n>=15)
		{
			for(i=0;i<4;i++) dMes[i]^=Rn_1[i];
			break;
		}
		for(i=0;i<4;i++) Rn_1[i]^=dMes[i];		
		for(i=0;i<4;i++) dMes[i]=dMes[i+4];		
		for(i=0;i<4;i++) dMes[i+4]=Rn_1[i];			
		ceDES_RightShift(dKeyC,LeftShift_table[15-n]);
	}	
	ceDES_Permutate(dMes,8,(unsigned char *)InverseIP_table,8);//64 bit output
};
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//dMes=Message to encrypt
//dKeyL=KeyLeft
//dKeyR=KeyRight
void ceTDES_Encrypt(unsigned char *dMes,unsigned char *dKeyL,unsigned char *dKeyR)
{
	ceDES_Encrypt(dMes,dKeyL);
	ceDES_Decrypt(dMes,dKeyR);
	ceDES_Encrypt(dMes,dKeyL);
};
//-----------------------------------------------------------------------------
//dMes=Message to decrypt
//dKeyL=KeyLeft
//dKeyR=KeyRight
void ceTDES_Decrypt(unsigned char *dMes,unsigned char *dKeyL,unsigned char *dKeyR)
{
	ceDES_Decrypt(dMes,dKeyL);
	ceDES_Encrypt(dMes,dKeyR);
	ceDES_Decrypt(dMes,dKeyL);
};
//-----------------------------------------------------------------------------

/////////////////////////////////////////////////////////////////////////////

#if defined(__cplusplus)
}
#endif

#endif // CEDES_H