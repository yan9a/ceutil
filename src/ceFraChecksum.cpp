/////////////////////////////////////////////////////////////////////////////
// Name:         ceFraChecksum.cpp
// Description:  Byte stuffing- sending and receiving frames with checksum
// Author:       Yan Naing Aye
// Ref: http://cool-emerald.blogspot.com/2011/05/byte-stuffing.html
/////////////////////////////////////////////////////////////////////////////
#include<stdio.h>
#include "ce/ceFraChecksum.h"
namespace ce {
//-----------------------------------------------------------------------------
ceFraChecksum::ceFraChecksum() :TxN(0), RxN(0), rState(CE_FRAME_IGNORE) {
	rb[0] = 0;
	tb[0] = 0;
}
//-----------------------------------------------------------------------------
char* ceFraChecksum::GetTxBuf() {
	return tb;
}
//-----------------------------------------------------------------------------
char* ceFraChecksum::GetRxBuf() {
	return rb;
}
//-----------------------------------------------------------------------------
//Prepare transmitting frame
size_t ceFraChecksum::SetTxFrame(char* d, size_t n)
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
uint8_t ceFraChecksum::Checksum(char* s, size_t len)
{
	uint8_t cs = 0; // check sum
	for (size_t i = 0; i < len; i++, s++) {
		cs ^= (uint8_t)(*s);
	}
	return cs;
}
//-----------------------------------------------------------------------------
//get number of transmitting bytes
size_t ceFraChecksum::GetTxN()
{
	return TxN;
}
//-----------------------------------------------------------------------------
//get number of receiving bytes
size_t ceFraChecksum::GetRxN()
{
	return RxN;
}
//-----------------------------------------------------------------------------
// process receiving char
// return RXN if a frame is successfully received, else retrun 0
size_t ceFraChecksum::ReceiveRxFrame(char c)
{
	unsigned int cs;
	unsigned int rxcs;
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

} // namespace ce 