/////////////////////////////////////////////////////////////////////////////
// Name:         ceFrame.cpp
// Description:  Byte stuffing- sending and receiving frames
// Author:       Yan Naing Aye
// Ref: http://cool-emerald.blogspot.com/2011/05/byte-stuffing.html
/////////////////////////////////////////////////////////////////////////////
#include<stdio.h>
#include "ce/ceFrame.h"
namespace ce {
//-----------------------------------------------------------------------------
ceFrame::ceFrame() :TxN(0), RxN(0), rState(CE_FRAME_IGNORE) {
	rb[0] = 0;
	tb[0] = 0;
}
//-----------------------------------------------------------------------------
char* ceFrame::GetTxBuf() {
	return tb;
}
//-----------------------------------------------------------------------------
char* ceFrame::GetRxBuf() {
	return rb;
}
//-----------------------------------------------------------------------------
//Prepare transmitting frame
size_t ceFrame::SetTxFrame(char* d, size_t n)
{
	unsigned int txcrc = 0xFFFF;//initialize crc
	char c;
	int i = 0, j = 0;
	tb[i++] = STX;//start of frame
	for (j = 0; j < n; j++) {
		c = d[j];
		if ((c == STX) || (c == ETX) || (c == DLE)) tb[i++] = (DLE);
		tb[i++] = c;
	}
	tb[i++] = (ETX);//end of frame

	txcrc = CRC16(d, n, txcrc);//calculate crc
	tb[i++] = txcrc & 0xFF;
	tb[i++] = (txcrc >> 8) & 0xFF;
	TxN = i;
	return TxN;
}
//-----------------------------------------------------------------------------
//Inputs
//s : pointer to input char string
//len: string len (maximum 255)
//crc: initial CRC value

//Output
//Returns calculated CRC
uint16_t ceFrame::CRC16(char* s, size_t len, uint16_t crc)
{
	//CRC Order: 16
	//CCITT(recommendation) : F(x)= x16 + x12 + x5 + 1
	//CRC Poly: 0x1021
	//Operational initial value:  0xFFFF
	//Final xor value: 0
	size_t i, j;
	for (i = 0; i < len; i++, s++) {
		crc ^= ((unsigned int)(*s) & 0xFF) << 8;
		for (j = 0; j < 8; j++) {
			if (crc & 0x8000) crc = (crc << 1) ^ 0x1021;
			else crc <<= 1;
		}
	}
	return (crc & 0xFFFF);//truncate last 16 bit
}
//-----------------------------------------------------------------------------
//get number of transmitting bytes
size_t ceFrame::GetTxN()
{
	return TxN;
}
//-----------------------------------------------------------------------------
//get number of receiving bytes
size_t ceFrame::GetRxN()
{
	return RxN;
}
//-----------------------------------------------------------------------------
// process receiving char
// return RXN if a frame is successfully received, else retrun 0
size_t ceFrame::ReceiveRxFrame(char c)
{
	static char b;
	unsigned int crc;
	unsigned int rxcrc = 0xFFFF;//initialize CRC
	switch (rState) {
	case CE_FRAME_RECEIVING:
		if (c == STX) { rState = CE_FRAME_RECEIVING; RxN = 0; }
		else if (c == ETX) { rState = CE_FRAME_RXCRC1; }
		else if (c == DLE) { rState = CE_FRAME_ESCAPE; }
		else { rb[RxN++] = c; }
		break;
	case CE_FRAME_ESCAPE:
		rb[RxN++] = c; rState = CE_FRAME_RECEIVING;
		break;
	case CE_FRAME_RXCRC1:
		b = c; rState = CE_FRAME_RXCRC2;
		break;
	case CE_FRAME_RXCRC2:
		rState = CE_FRAME_IGNORE;
		crc = ((int)c << 8 | ((int)b & 0xFF)) & 0xFFFF;//get received crc
		rxcrc = CRC16(rb, RxN, rxcrc);//calculate crc
		//printf("crc: %x  rxcrc:%x \n",crc,rxcrc);
		if (rxcrc == crc) { return RxN; }//if crc is correct            
		else { RxN = 0; }//discard the frame
		break;
	default: //case CE_FRAME_IGNORE:
		if (c == STX) { rState = CE_FRAME_RECEIVING; RxN = 0; }
		break;
	}
	return 0;
}

} // namespace ce 