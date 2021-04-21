/////////////////////////////////////////////////////////////////////////////
// Name:         ceModbus.h
// Description:  Sending and receiving modbus frames
//  Modbus RTU frame format (primarily used on asynchronous serial data lines like RS-485/EIA-485)
// Author:       Yan Naing Aye
// Ref: http://cool-emerald.blogspot.com
/////////////////////////////////////////////////////////////////////////////

#ifndef CE_MODBUS_H
#define CE_MODBUS_H
#include "ce/ceFrame.h"
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
} // namespace ce 

#endif // CE_MODBUS_H