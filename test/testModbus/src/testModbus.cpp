// File: testModbus.cpp
// Description: Modbus frame sending and receiving console program for Windows and Linux
// WebSite: http://cool-emerald.blogspot.com
// MIT License (https://opensource.org/licenses/MIT)
// Copyright (c) 2021 Yan Naing Aye

#include <stdio.h>
#include "ce/ceSerial.h"
#include "ce/ceModbus.h"
#include <vector>
using namespace std;
using namespace ce;

int main()
{
#ifdef CE_WINDOWS
	ceSerial com("\\\\.\\COM17", 9600, 8, 'N', 1); // Windows
#else
	ceSerial com("/dev/ttyS0", 9600, 8, 'N', 1); // Linux
#endif

	printf("Opening port %s.\n", com.GetPort().c_str());
	if (com.Open() == 0) {
		printf("OK.\n");
	}
	else {
		printf("Error.\n");
		return 1;
	}
	ceModbus m;
	m.SetControl(1,1,3);
	// m.SetReadStatus(1, 1, 4);
	printf("Writing %z bytes.\n", m.GetTxN());

	bool successFlag;
	successFlag = com.Write(m.GetTxBuf(), (long)m.GetTxN());
	
	printf("Waiting ...\n");
	ceSerial::Delay(1000); // delay to wait for reply

	printf("Reading.\n");
	successFlag = true;
	
	printf("Rx: ");
	while (successFlag) {
		char c = com.ReadChar(successFlag); // read a char
		if (successFlag) {
			printf("%02X ", (unsigned int)c & 0xFF);
			if (m.ReceiveRxFrame(c)) {
				char* buf = m.GetRxBuf();
				size_t rn = m.GetRxN();
				if (buf[1] == 6) {
					printf("\nReceived control reply\n");
				}
				else if (buf[1] == 3) {
					printf("\nReceived read reply\n");
					std::vector<uint16_t> rstatus = m.GetStatus(buf,rn);
					for (uint16_t& s : rstatus) {
						printf("Status: %d\n",s);
					}
				}
			}
		}
	}
	printf("\n");

	printf("Closing port %s.\n", com.GetPort().c_str());
	com.Close();
	return 0;
}