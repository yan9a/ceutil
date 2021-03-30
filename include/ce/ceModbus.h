/////////////////////////////////////////////////////////////////////////////
// Name:         ceModbus.h
// Description:  Sending and receiving modbus frames
//  Modbus RTU frame format (primarily used on asynchronous serial data lines like RS-485/EIA-485)
// Author:       Yan Naing Aye
// Ref: http://cool-emerald.blogspot.com
/////////////////////////////////////////////////////////////////////////////

#ifndef CE_MODBUS_H
#define CE_MODBUS_H

#define CR 0x0D
#define LF 0x0A
#define CE_MODBUS_TX_BUF_SIZE 64
#define CE_MODBUS_RX_BUF_SIZE 64
#define CE_NUMBER_OF_TICKS_TO_RESET 8
#include <cstdint>
#include <vector>
namespace ce {
//-----------------------------------------------------------------------------
class ceModbus {
private:
protected:
	size_t TxN;//number of transmitting bytes
	size_t RxN;//number of receiving bytes
	char tb[CE_MODBUS_TX_BUF_SIZE];//transmit buffer
	char rb[CE_MODBUS_RX_BUF_SIZE];//receiving data
	size_t _count; // receiving count
	size_t _frameSize;// frame size
	int _tick_n;
public:
	ceModbus();
	size_t SetTxFrame(const char* d, size_t n);
	uint16_t CalCRC16(char* s, size_t len, uint16_t crc);
	size_t GetTxN();
	size_t GetRxN();
	size_t GetRxFrame(char c);//get receiving frame from received char
	char* GetTxBuf();
	char* GetRxBuf();
	int Tick();// to reset frame receiving after CE_NUMBER_OF_TICKS_TO_RESET of no char rx 
	void SetCmd(std::vector<uint8_t> v);
	void SetCmd(uint8_t slaveid, uint8_t func, uint16_t addr, std::vector<uint8_t> data);
	void SetControl(uint8_t slaveid, uint16_t addr, uint8_t command, uint8_t delay=0);
	void SetReadStatus(uint8_t slaveid, uint16_t startAddr, uint16_t len);
	std::vector<uint16_t> GetStatus(char* d, size_t n);
	std::vector<char> GetTxVec();
	std::vector<char> GetRxVec();
};
//-----------------------------------------------------------------------------
} // namespace ce 

#endif // CE_MODBUS_H