/////////////////////////////////////////////////////////////////////////////
// Name:         cePCA9535.h
// Description:  cePCA9535 - Digital IO chip 
// Author:       Yan Naing Aye
/////////////////////////////////////////////////////////////////////////////
#include <cstdint>
#include "ce/ceI2C.h"
#ifndef cePCA9535_H
#define cePCA9535_H
#ifndef PCA9535_REGISTERS
    #define PCA9535_REGISTERS
    #define PCA9535_INPUT_P0_ADDR 0
    #define PCA9535_INPUT_P1_ADDR 1
    #define PCA9535_OUTPUT_P0_ADDR 2
    #define PCA9535_OUTPUT_P1_ADDR 3
    #define PCA9535_POLARITY_P0_ADDR 4
    #define PCA9535_POLARITY_P1_ADDR 5
    #define PCA9535_CONF_P0_ADDR 6
    #define PCA9535_CONF_P1_ADDR 7
#endif
namespace ce {

class cePCA9535 {
public:
	cePCA9535(uint8_t i2cBusNo, uint8_t Addr);
	~cePCA9535();
    bool SetReg(uint8_t rVal, uint8_t rAddr);
    bool GetReg(uint8_t& rVal, uint8_t rAddr);
    bool Init(unsigned char outp0,unsigned char outp1, 
		unsigned char polarity0,unsigned char polarity1, 
		unsigned char conf0,unsigned char conf1);
private:
	uint8_t _bus; // i2c bus number
	uint8_t _addr; // addr2 jumper open (0) or close (1)
    bool SetReg(uint8_t chipAddr, uint8_t rVal, uint8_t rAddr, uint8_t i2cBus);
    bool GetReg(uint8_t chipAddr, uint8_t& rVal, uint8_t rAddr, uint8_t i2cBus);

    // initialize PCA9535
	bool Init(unsigned char chipAddr,
		unsigned char outp0,unsigned char outp1, 
		unsigned char polarity0,unsigned char polarity1, 
		unsigned char conf0,unsigned char conf1, int i2cBus);
};

} // namespace ce
#endif // cePCA9535_H
