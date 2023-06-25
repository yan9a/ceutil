// Description: Simple functions to calculate DES (Data Encryption Standard) and 3DES (Triple DES)
//  Class implementation of the following repository: https://github.com/yan9a/ceDES
// WebSite: http://cool-emerald.blogspot.com
// MIT License (https://opensource.org/licenses/MIT)
// Copyright (c) 2023 Yan Naing Aye

// Related links
// https://www.rapidtables.com/convert/number/ascii-to-hex.html
// https://emvlab.org/descalc/
// https://paymentcardtools.com/crypto-calculators/des-calculator
// https://github.com/yan9a/cecpp/blob/master/des_cryptopp/main.cpp

#include "ce/ceDES.h"
namespace ce {
//-----------------------------------------------------------------------------
//dMes=Message to encrypt
//dKey=Key
void ceDES::Encrypt(unsigned char *dMes,unsigned char *dKey)
{
	unsigned char n,i;	
	unsigned char dKeyC[8];
	for(i=0;i<8;i++) dKeyC[i]=dKey[i];//copy key
	
	this->Permutate(dMes,8,(unsigned char *)IP_table,8);//64 bit output
	this->Permutate(dKeyC,8,(unsigned char *)PC1_table,7);//56 bit output	
	for(n=0;n<16;n++)
	{
		this->LeftShift(dKeyC,LeftShift_table[n]);
		for(i=0;i<7;i++) Kn[i]=dKeyC[i];
		this->Permutate(Kn,7,(unsigned char *)PC2_table,6);//48 bit output			
		for(i=0;i<4;i++) Rn_1[i]=dMes[i+4];	
		this->F(Rn_1,Kn);	
		
		if(n>=15)
		{
			for(i=0;i<4;i++) dMes[i]^=Rn_1[i];
			break;
		}
		for(i=0;i<4;i++) Rn_1[i]^=dMes[i];		
		for(i=0;i<4;i++) dMes[i]=dMes[i+4];
		for(i=0;i<4;i++) dMes[i+4]=Rn_1[i];			
	}	
	this->Permutate(dMes,8,(unsigned char *)InverseIP_table,8);//64 bit output
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//dMes=Message to decrypt
//dKey=Key
void ceDES::Decrypt(unsigned char *dMes,unsigned char *dKey)
{
	unsigned char n,i;	
	unsigned char dKeyC[8];
	for(i=0;i<8;i++) dKeyC[i]=dKey[i];//copy key
	
	this->Permutate(dMes,8,(unsigned char *)IP_table,8);//64 bit output
	this->Permutate(dKeyC,8,(unsigned char *)PC1_table,7);//56 bit output
	
	for(n=0;n<16;n++)
	{		
		for(i=0;i<7;i++) Kn[i]=dKeyC[i];
		this->Permutate(Kn,7,(unsigned char *)PC2_table,6);//48 bit output			
		for(i=0;i<4;i++) Rn_1[i]=dMes[i+4];			
		this->F(Rn_1,Kn);	
		
		if(n>=15)
		{
			for(i=0;i<4;i++) dMes[i]^=Rn_1[i];
			break;
		}
		for(i=0;i<4;i++) Rn_1[i]^=dMes[i];		
		for(i=0;i<4;i++) dMes[i]=dMes[i+4];		
		for(i=0;i<4;i++) dMes[i+4]=Rn_1[i];			
		this->RightShift(dKeyC,LeftShift_table[15-n]);
	}	
	this->Permutate(dMes,8,(unsigned char *)InverseIP_table,8);//64 bit output
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//pData  =input to permutate
//ni     =number of input bytes
//pTable =permutation table
//no     =number of output bytes
void ceDES::Permutate(unsigned char *pData,unsigned char ni,
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
	
}
//-----------------------------------------------------------------------------
//nKey -Key to shift
//b -number of bits
//priority to speed rather than code size
void ceDES::LeftShift(unsigned char *nKey,unsigned char b)
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
}
//-----------------------------------------------------------------------------
//nKey -Key to shift
//b -number of bits
//written for speed
void ceDES::RightShift(unsigned char *nKey,unsigned char b)
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
}
//-----------------------------------------------------------------------------
//written for speed
void ceDES::Expand(unsigned char *R)
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
}
//-----------------------------------------------------------------------------
//written for speed
void ceDES::F(unsigned char *R,unsigned char *K)
{
	unsigned char i,r,c;
	
	//special function for expansion to improve speed
	this->Expand(R);	
	
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
	this->Permutate(R,4,(unsigned char *)P_table,4);//32 bit output
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//dMes=Message to encrypt
//dKeyL=KeyLeft
//dKeyR=KeyRight
void ceDES::Encrypt(unsigned char *dMes,unsigned char *dKeyL,unsigned char *dKeyR)
{
	this->Encrypt(dMes,dKeyL);
	this->Decrypt(dMes,dKeyR);
	this->Encrypt(dMes,dKeyL);
}
//-----------------------------------------------------------------------------
//dMes=Message to decrypt
//dKeyL=KeyLeft
//dKeyR=KeyRight
void ceDES::Decrypt(unsigned char *dMes,unsigned char *dKeyL,unsigned char *dKeyR)
{
	this->Decrypt(dMes,dKeyL);
	this->Encrypt(dMes,dKeyR);
	this->Decrypt(dMes,dKeyL);
}
//-----------------------------------------------------------------------------
} // namespace ce 