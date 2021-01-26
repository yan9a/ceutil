/////////////////////////////////////////////////////////////////////////////
// Name:         cePCA9535.cpp
// Description:  cePCA9535 - Digital IO chip
// Author:       Yan Naing Aye
// Date:         2019 October 09
/////////////////////////////////////////////////////////////////////////////
#include "ce/cePCA9535.h"

namespace ce {

cePCA9535::cePCA9535(uint8_t i2cBusNo, uint8_t Addr) : _bus(i2cBusNo),_addr(Addr)
{

}

cePCA9535::~cePCA9535()
{

}

bool cePCA9535::SetReg(uint8_t rVal, uint8_t rAddr)
{
	return this->SetReg(_addr,rVal,rAddr,_bus);
}

bool cePCA9535::GetReg(uint8_t& rVal, uint8_t rAddr)
{
	return this->GetReg(_addr,rVal,rAddr,_bus);
}

bool cePCA9535::Init(unsigned char outp0,unsigned char outp1, 
	unsigned char polarity0,unsigned char polarity1, 
	unsigned char conf0,unsigned char conf1)
{
	return this->Init(_addr, 
		outp0, outp1,
		polarity0, polarity1,
		conf0, conf1, _bus);
}

bool cePCA9535::SetReg(uint8_t chipAddr, uint8_t rVal, uint8_t rAddr, uint8_t i2cBus)
{
	char d[] = { 0,0 };
	ce::ceI2C chip1(i2cBus, chipAddr);
	d[0] = rAddr;
	d[1] = rVal;
	bool r = chip1.Write(d, 2);	// send new register data
	chip1.Close();
	if (r == false) {
		printf("cePCA9535 I2C error at bus = %d, addr = 0x%X\n",i2cBus,chipAddr);
		// perror("cePCA9535 I2C error");
	}
	return r;
}

bool cePCA9535::GetReg(uint8_t chipAddr, uint8_t& rVal, uint8_t rAddr, uint8_t i2cBus)
{
	char d[] = { 0,0 };
	ce::ceI2C chip1(i2cBus, chipAddr);
	d[0] = rAddr;
	bool r = chip1.Write(d, 1);	// send new register data
	r = r && chip1.Read(d, 1);
	chip1.Close();
	if (r) {
		rVal = d[0];
	}
	else {
		printf("cePCA9535 I2C error at bus = %d, addr = 0x%X\n",i2cBus,chipAddr);
		// perror("cePCA9535 I2C error");
	}
	return r;
}

// initialize PCA9535
bool cePCA9535::Init(unsigned char chipAddr, 
	unsigned char outp0, unsigned char outp1,
	unsigned char polarity0, unsigned char polarity1,
	unsigned char conf0, unsigned char conf1, int i2cbus)
{
	bool r = true;
	r = r && this->SetReg(chipAddr, polarity0, PCA9535_POLARITY_P0_ADDR, i2cbus);
	r = r && this->SetReg(chipAddr, polarity1, PCA9535_POLARITY_P1_ADDR, i2cbus);
	r = r && this->SetReg(chipAddr, conf0, PCA9535_CONF_P0_ADDR, i2cbus);
	r = r && this->SetReg(chipAddr, conf1, PCA9535_CONF_P1_ADDR, i2cbus);
	r = r && this->SetReg(chipAddr, outp0, PCA9535_OUTPUT_P0_ADDR, i2cbus);
	r = r && this->SetReg(chipAddr, outp1, PCA9535_OUTPUT_P1_ADDR, i2cbus);
	return r;
}

} // namespace ce