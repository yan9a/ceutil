/////////////////////////////////////////////////////////////////////////////
// Name:         cefracs.h
// Description:  Byte stuffing- sending and receiving frames with checksum
// Author:       Yan Naing Aye
// Ref: http://cool-emerald.blogspot.com/2011/05/byte-stuffing.html
/////////////////////////////////////////////////////////////////////////////

#ifndef CE_FRACHECKSUM_H
#define CE_FRACHECKSUM_H

#include <cstdint>
#include<cstdio>

#define STX 0x02
#define ETX 0x03
#define DLE 0x10

#define CE_FRAME_TX_BUF_SIZE 1024
#define CE_FRAME_RX_BUF_SIZE 1024

// frame state definition 
#define CE_FRAME_IGNORE 0
#define CE_FRAME_RECEIVING 1
#define CE_FRAME_ESCAPE 2
#define CE_FRAME_CHKSUM 3

namespace ce {

//-----------------------------------------------------------------------------
class ceFraChecksum {
	int rState;
protected:
	size_t TxN;//number of transmitting bytes
	size_t RxN;//number of receiving bytes
	char tb[CE_FRAME_TX_BUF_SIZE];//transmit buffer
	char rb[CE_FRAME_RX_BUF_SIZE];//receiving data
public:
	ceFraChecksum();
	size_t GetTxN();
	size_t GetRxN();
	char* GetTxBuf();
	char* GetRxBuf();
	virtual size_t SetTxFrame(char* d, size_t n);
	virtual uint8_t Checksum(char* s, size_t len);
	virtual size_t ReceiveRxFrame(char c);//get receiving frame from received char
};
//-----------------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
inline ceFraChecksum::ceFraChecksum() :TxN(0), RxN(0), rState(CE_FRAME_IGNORE) {
	rb[0] = 0;
	tb[0] = 0;
}
//-----------------------------------------------------------------------------
inline char* ceFraChecksum::GetTxBuf() {
	return tb;
}
//-----------------------------------------------------------------------------
inline char* ceFraChecksum::GetRxBuf() {
	return rb;
}
//-----------------------------------------------------------------------------
//Prepare transmitting frame
inline size_t ceFraChecksum::SetTxFrame(char* d, size_t n)
{
	char c;
	int i = 0, j = 0;
	tb[i++] = STX;//start of frame
	for (j = 0; j < n; j++) {
		c = d[j];
		if ((c == STX) || (c == ETX) || (c == DLE)) tb[i++] = (DLE);
		tb[i++] = c;
	}
	tb[i++] = (ETX);//end of frame
	tb[i++] = Checksum(d, n);//calculate CS
	TxN = i;
	return TxN;
}
//-----------------------------------------------------------------------------
//Inputs
//s : pointer to input char string
//len: string len 

//Output
//Returns calculated Checksum
inline uint8_t ceFraChecksum::Checksum(char* s, size_t len)
{
	uint8_t cs = 0; // check sum
	for (size_t i = 0; i < len; i++, s++) {
		cs ^= (uint8_t)(*s);
	}
	return cs;
}
//-----------------------------------------------------------------------------
//get number of transmitting bytes
inline size_t ceFraChecksum::GetTxN()
{
	return TxN;
}
//-----------------------------------------------------------------------------
//get number of receiving bytes
inline size_t ceFraChecksum::GetRxN()
{
	return RxN;
}
//-----------------------------------------------------------------------------
// process receiving char
// return RXN if a frame is successfully received, else retrun 0
inline size_t ceFraChecksum::ReceiveRxFrame(char c)
{
	uint8_t cs;
	uint8_t rxcs;
	switch (rState) {
	case CE_FRAME_RECEIVING:
		if (c == STX) { rState = CE_FRAME_RECEIVING; RxN = 0; }
		else if (c == ETX) { rState = CE_FRAME_CHKSUM; }
		else if (c == DLE) { rState = CE_FRAME_ESCAPE; }
		else { rb[RxN++] = c; }
		break;
	case CE_FRAME_ESCAPE:
		rb[RxN++] = c; rState = CE_FRAME_RECEIVING;
		break;
	case CE_FRAME_CHKSUM:
		rxcs = c;
		rState = CE_FRAME_IGNORE;
		cs = Checksum(rb, RxN);//calculate crc
		//printf("cs: %x  rxcs: %x \n",cs,rxcs);
		if (rxcs == cs) { return RxN; }//if cs is correct            
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

#endif // CE_FRACHECKSUM_H