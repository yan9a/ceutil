// File: testSerial.cpp
// Description: Serial communication console program for Windows and Linux
// WebSite: http://cool-emerald.blogspot.sg/2017/05/serial-port-programming-in-c-with.html
// MIT License (https://opensource.org/licenses/MIT)
// Copyright (c) 2018 Yan Naing Aye

#include <stdio.h>
#include "ce/ceserial.h"
using namespace std;

int main()
{
#ifdef CE_WINDOWS
	ceSerial com("\\\\.\\COM6", 9600, 8, 'N', 1); // Windows
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

	bool successFlag;
	printf("Writing.\n");
	char s[] = "Hello";
	successFlag = com.Write(s); // write string
	successFlag = com.WriteChar('!'); // write a character

	printf("Waiting 3 seconds.\n");
	ceSerial::Delay(3000); // delay to wait for a character

	printf("Reading.\n");

	successFlag = true;
	printf("Rx: ");
	while (successFlag) {
		char c = com.ReadChar(successFlag); // read a char
		if (successFlag) printf("%c", c);
	}
	printf("\n");

	printf("Closing port %s.\n", com.GetPort().c_str());
	com.Close();
	return 0;
}