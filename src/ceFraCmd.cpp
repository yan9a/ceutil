/////////////////////////////////////////////////////////////////////////////
// Name:         ceFraCmd.cpp
// Description:  Byte stuffing- sending and receiving commands as frames
// Author:       Yan Naing Aye
// Ref: http://cool-emerald.blogspot.com/2011/05/byte-stuffing.html
/////////////////////////////////////////////////////////////////////////////
#include<stdio.h>
#include "ce/ceFraCmd.h"
namespace ce {
//-----------------------------------------------------------------------------
ceFraCmd::ceFraCmd():ceFrame(){

}
//-----------------------------------------------------------------------------
//Prepare transmitting frame
size_t ceFraCmd::SetTxFrame(char* d, size_t n)
{
	int i = 0;
	if (n >= CE_FRAME_TX_BUF_SIZE) n = CE_FRAME_TX_BUF_SIZE - 1; //discard policy
	for (int j = 0; j < n; j++) {
		tb[i++] = d[j];
	}
	tb[i++] = 0x0D;//end of frame
	TxN = i;
	return TxN;
}
//-----------------------------------------------------------------------------
// process receiving char
// return RXN if a frame is successfully received, else retrun 0
size_t ceFraCmd::ReceiveRxFrame(char ch)
{
	static int n = 0;
	if (ch >= 32 && ch <= 126) {
		this->rb[n++] = ch;
		this->RxN = 0;
	}

	if (ch == 0x0D || n >= (CE_FRAME_RX_BUF_SIZE -1)) {
		this->RxN = n;
		this->rb[n] = 0;//null termination
		n = 0;
		return this->RxN;
	}
	return 0;
}

} // namespace ce 