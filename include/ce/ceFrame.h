/////////////////////////////////////////////////////////////////////////////
// Name:         ceFrame.h
// Description:  Byte stuffing- sending and receiving frames
// Author:       Yan Naing Aye
// Ref: http://cool-emerald.blogspot.com/2011/05/byte-stuffing.html
/////////////////////////////////////////////////////////////////////////////

#ifndef CE_FRAME_H
#define CE_FRAME_H

#define STX 0x02
#define ETX 0x03
#define DLE 0x10

#define CE_FRAME_TX_BUF_SIZE 1024
#define CE_FRAME_RX_BUF_SIZE 1024

namespace ce {
enum CE_FRAME_RX_STATE { CE_FRAME_IGNORE, CE_FRAME_RECEIVING, CE_FRAME_ESCAPE, CE_FRAME_RXCRC1, CE_FRAME_RXCRC2 };
//-----------------------------------------------------------------------------
class ceFrame {
	CE_FRAME_RX_STATE rState;
protected:
	int TxN;//number of transmitting bytes
	int RxN;//number of receiving bytes
	char tb[CE_FRAME_TX_BUF_SIZE];//transmit buffer
	char rb[CE_FRAME_RX_BUF_SIZE];//receiving data
public:
	ceFrame();
	int setTxFrame(char* d, int n);
	unsigned int CRC16CCITT_Calculate(char* s, unsigned char len, unsigned int crc);
	int getTxN();
	int getRxN();
	int receiveRxFrame(char c);//get receiving frame from received char
	char* getTxBuf();
	char* getRxBuf();
};
//-----------------------------------------------------------------------------
} // namespace ce 

#endif // CE_FRAME_H