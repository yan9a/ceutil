#include "ce/ceUtil.h"
using namespace ce;

int main(){
	cePCA9535 chip1(1,0x21);// bus = 1, addr = 0x21

	chip1.Init(0x00, 0xFF, // output p0 all 0, and p1 all 1
		0x00, 0x00, // p0 active high , p1 active high also
		0x00, 0x00); // configure all as output

	uint8_t o2 = 0x00;
	int i=0;
	while(1){
		wxMilliSleep(1000);
		o2 ^= 0x3F;
		chip1.SetReg(o2,PCA9535_OUTPUT_P1_ADDR);
		printf(".");
		fflush(stdout);
		if(++i>10){
			printf("\n");
			break;
		}
	}
	return 0;
}
