/////////////////////////////////////////////////////////////////////////////
// Name:         ceFrame.h
// Description:  Byte stuffing- sending and receiving frames
// Author:       Yan Naing Aye
// Ref: http://cool-emerald.blogspot.com/2011/05/byte-stuffing.html
/////////////////////////////////////////////////////////////////////////////

#ifndef CE_FRAME_H
#define CE_FRAME_H

#include <cstdint>

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
} // namespace ce 

#endif // CE_FRAME_H