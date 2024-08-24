/////////////////////////////////////////////////////////////////////////////
// Name:         ceframe.h
// Description:  Byte stuffing- sending and receiving frames
// Author:       Yan Naing Aye
// Ref: http://cool-emerald.blogspot.com/2011/05/byte-stuffing.html
/////////////////////////////////////////////////////////////////////////////

#ifndef CE_FRAME_H
#define CE_FRAME_H

#include <cstdint>
#include <cstdio>

#define STX 0x02
#define ETX 0x03
#define DLE 0x10

#define CE_FRAME_TX_BUF_SIZE 1024
#define CE_FRAME_RX_BUF_SIZE 1024

// frame state definition 
#define CE_FRAME_IGNORE 0
#define CE_FRAME_RECEIVING 1
#define CE_FRAME_ESCAPE 2
#define CE_FRAME_RXCRC1 3
#define CE_FRAME_RXCRC2 4

namespace ce {

//-----------------------------------------------------------------------------
class ceFrame {
	int rState;
protected:
	size_t TxN;//number of transmitting bytes
	size_t RxN;//number of receiving bytes
	char tb[CE_FRAME_TX_BUF_SIZE];//transmit buffer
	char rb[CE_FRAME_RX_BUF_SIZE];//receiving data
public:
	ceFrame();
	size_t GetTxN();
	size_t GetRxN();
	char* GetTxBuf();
	char* GetRxBuf();
	virtual size_t SetTxFrame(char* d, size_t n);
	virtual uint16_t CRC16(char* s, size_t len, uint16_t crc);
	virtual size_t ReceiveRxFrame(char c);//get receiving frame from received char
};
//-----------------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
inline ceFrame::ceFrame() :TxN(0), RxN(0), rState(CE_FRAME_IGNORE) {
	rb[0] = 0;
	tb[0] = 0;
}
//-----------------------------------------------------------------------------
inline char* ceFrame::GetTxBuf() {
	return tb;
}
//-----------------------------------------------------------------------------
inline char* ceFrame::GetRxBuf() {
	return rb;
}
//-----------------------------------------------------------------------------
//Prepare transmitting frame
inline size_t ceFrame::SetTxFrame(char* d, size_t n)
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
inline uint16_t ceFrame::CRC16(char* s, size_t len, uint16_t crc)
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
inline size_t ceFrame::GetTxN()
{
	return TxN;
}
//-----------------------------------------------------------------------------
//get number of receiving bytes
inline size_t ceFrame::GetRxN()
{
	return RxN;
}
//-----------------------------------------------------------------------------
// process receiving char
// return RXN if a frame is successfully received, else retrun 0
inline size_t ceFrame::ReceiveRxFrame(char c)
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

	if (RxN >= CE_FRAME_RX_BUF_SIZE) {
		RxN = 0;
		rState = CE_FRAME_IGNORE;
	}

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
} // namespace ce 

#endif // CE_FRAME_H