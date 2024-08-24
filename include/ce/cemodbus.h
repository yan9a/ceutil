/////////////////////////////////////////////////////////////////////////////
// Name:         ceModbus.h
// Description:  Sending and receiving modbus frames
//  Modbus RTU frame format (primarily used on asynchronous serial data lines like RS-485/EIA-485)
// Author:       Yan Naing Aye
// Ref: http://cool-emerald.blogspot.com
/////////////////////////////////////////////////////////////////////////////

#ifndef CE_MODBUS_H
#define CE_MODBUS_H
#include "ce/ceframe.h"
#define CR 0x0D
#define LF 0x0A
#define CE_NUMBER_OF_TICKS_TO_RESET 10
#include <cstdint>
#include <vector>
namespace ce {
//-----------------------------------------------------------------------------
class ceModbus : public ceFrame {
private:
protected:
	size_t _count; // receiving count
	size_t _frameSize;// frame size
	int _tick_n;
	int _reset_tick;
public:
	ceModbus();
	size_t SetTxFrame(char* d, size_t n) override;
	uint16_t CRC16(char* s, size_t len, uint16_t crc) override;
	size_t ReceiveRxFrame(char c) override;//get receiving frame from received char
	int Tick();// to reset frame receiving after CE_NUMBER_OF_TICKS_TO_RESET of no char rx 
	void SetCmd(std::vector<uint8_t> v);
	void SetCmd(uint8_t slaveid, uint8_t func, uint16_t addr, std::vector<uint8_t> data);
	void SetControl(uint8_t slaveid, uint16_t addr, uint8_t command, uint8_t delay=0);
	void SetReadStatus(uint8_t slaveid, uint16_t startAddr, uint16_t len);
	std::vector<uint16_t> GetStatus(char* d, size_t n);
	std::vector<char> GetTxVec();
	std::vector<char> GetRxVec();
	void SetResetTickCount(int n);
	int GetResetTickCount();
};
//-----------------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
inline ceModbus::ceModbus() :ceFrame(), _count(0), _frameSize(8), _tick_n(0), _reset_tick(CE_NUMBER_OF_TICKS_TO_RESET) {
}
//-----------------------------------------------------------------------------
//Prepare transmitting frame
inline size_t ceModbus::SetTxFrame(char* d, size_t n)
{
	if (n > (CE_FRAME_TX_BUF_SIZE - 2)) return 0;// size error
	uint16_t txcrc = 0xFFFF;//initialize crc
	char c;
	size_t i = 0, j = 0;
	for (j = 0; j < n; j++) {
		c = d[j];
		// no need to check c in building Modbus frame
		this->tb[i++] = c;
	}
	txcrc = this->CRC16((char*)d, n, txcrc);//calculate crc
	this->tb[i++] = txcrc & 0xFF;
	this->tb[i++] = (txcrc >> 8) & 0xFF;
	this->TxN = i;
	return TxN;
}
//-----------------------------------------------------------------------------
//Inputs
//s : pointer to input char string
//len: string len (maximum 255)
//crc: initial CRC value

//Output
//Returns calculated CRC

// Ref: http://cool-emerald.blogspot.com/2009/09/crc-calculation-in-vb-and-c.html
inline uint16_t ceModbus::CRC16(char* s, size_t len, uint16_t crc)
{
	//CRC Order: 16
	//CRC Poly: 0x8005 <=> A001
	//Operational initial value:  0xFFFF
	//Final xor value: 0
	size_t i, j;
	for (i = 0; i < len; i++, s++) {
		crc ^= ((unsigned int)(*s)) & 0xFF;
		for (j = 0; j < 8; j++) {
			if (crc & 0x0001) crc = (crc >> 1) ^ 0xA001;
			else crc >>= 1;
		}
	}
	return (crc & 0xFFFF);//truncate last 16 bit
}
//-----------------------------------------------------------------------------
// process receiving char
// return RXN if a frame is successfully received, else retrun 0
// CRC16 is also included
inline size_t ceModbus::ReceiveRxFrame(char c)
{
	// accept any value even 0x0D to get 8 bytes 
	//  (not using start and end 28 bit length mark condition)
	this->_tick_n = 0; // reset for every rx ch
	this->RxN = 0;
	this->rb[this->_count++] = c;

	if (this->_count == 3) {
		// check function
		if (this->rb[1] == 0x06) {
			this->_frameSize = 8; // reply for control cmd
		}
		else if (this->rb[1] == 0x03) {
			uint8_t len = (uint8_t)this->rb[2];
			this->_frameSize = 5 + len; // reply for read cmd
		}
	}

	// instead use frame size for particular frame
	if (this->_count >= this->_frameSize) {
		this->rb[this->_count] = 0;//null termination
		this->RxN = this->_count;
		this->_count = 0;
		this->_frameSize = 8;// default
		this->_tick_n = 0; // will clear RxN when time out
		// for (int i = 0; i < this->RxN; i++) printf("%02X ", (unsigned int)this->rb[i] & 0xFF);
		// uint16_t rxcrc = ((uint16_t)this->rb[this->RxN-1] << 8 | ((uint16_t)this->rb[this->RxN - 2] & 0xFF)) & 0xFFFF;//get received crc
		uint16_t computed_crc = 0xFFFF;//initialize CRC
		computed_crc = this->CRC16(rb, RxN, computed_crc);//calculate crc
		// printf("\nComputed crc: %02X \n",computed_crc);
		if (computed_crc == 0) {
			//this->RxN -= 2; 
			return (this->RxN); //if crc is correct return bytes including crc 
		}
		else { this->RxN = 0; }//discard the frame
	}
	return 0;
}

// to reset frame receiving after CE_NUMBER_OF_TICKS_TO_RESET of no char rx 
// return =  0 : if no reset, 1 : if reset
inline int ceModbus::Tick()
{
	if (this->_tick_n <= this->_reset_tick) {
		this->_tick_n++;
	}
	else {
		this->_count = 0;
		this->_frameSize = 8;// default
		this->RxN = 0;
	}
	return this->_tick_n;
}

// command vector without CRC16
// CRC16 will be calculated and appended
inline void ceModbus::SetCmd(std::vector<uint8_t> v)
{
	this->SetTxFrame((char*)v.data(), v.size());
}

inline void ceModbus::SetCmd(uint8_t slaveid, uint8_t func, uint16_t addr, std::vector<uint8_t> data)
{
	uint8_t ah = uint8_t((addr >> 8) & 0xFF);
	uint8_t al = uint8_t(addr & 0xFF);
	std::vector<uint8_t> v = { slaveid,func,ah,al };
	v.insert(v.end(), data.begin(), data.end());
	this->SetCmd(v);
}

inline void ceModbus::SetControl(uint8_t slaveid, uint16_t addr, uint8_t command, uint8_t delay)
{
	uint8_t ah = uint8_t((addr >> 8) & 0xFF);
	uint8_t al = uint8_t(addr & 0xFF);
	std::vector<uint8_t> v = { slaveid,0x06,ah,al,command,delay };
	this->SetCmd(v);
}

inline void ceModbus::SetReadStatus(uint8_t slaveid, uint16_t startAddr, uint16_t len)
{
	uint8_t ah = uint8_t((startAddr >> 8) & 0xFF);
	uint8_t al = uint8_t(startAddr & 0xFF);
	uint8_t lh = uint8_t((len >> 8) & 0xFF);
	uint8_t ll = uint8_t(len & 0xFF);
	std::vector<uint8_t> v = { slaveid,0x03,ah,al,lh,ll };
	this->SetCmd(v);
}

// extract vector of status from received byte array
// crc16 bytes will be neglected if included
inline std::vector<uint16_t> ceModbus::GetStatus(char* d, size_t n)
{
	std::vector<uint16_t> v;
	if (n < 3) {
		// perror("Modbus frame error in getting status");
		return v;
	}

	if (d[1] != 0x03) {
		// perror("Frame must be Modbus read status return to get status");
		return v;
	}

	uint8_t len = (uint8_t)d[2]; // get len
	if (n < ((size_t)len + 3)) {
		// perror("Modbus frame length error in getting status");
		return v;
	}

	uint16_t s;
	uint8_t count = len >> 1;// 2 bytes each
	for (uint8_t i = 0; i < count; i++) {
		// for big endian
		s = (uint8_t)d[3 + ((uint64_t)i << 1)];
		s <<= 8;
		s |= (uint8_t)d[4 + ((uint64_t)i << 1)];
		v.push_back(s);
	}
	return v;
}

inline std::vector<char> ceModbus::GetTxVec()
{
	std::vector<char> v((char*)tb, (char*)tb + this->TxN);
	return v;
}

inline std::vector<char> ceModbus::GetRxVec()
{
	std::vector<char> v((char*)rb, (char*)rb + this->RxN);
	return v;
}

inline void ceModbus::SetResetTickCount(int n)
{
	this->_reset_tick = n;
}

inline int ceModbus::GetResetTickCount()
{
	return this->_reset_tick;
}

/////////////////////////////////////////////////////////////////////////////
} // namespace ce 

#endif // CE_MODBUS_H