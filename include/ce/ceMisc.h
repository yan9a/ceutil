/////////////////////////////////////////////////////////////////////////////
// Name:         ceMisc.h
// Description:  utility module
// Author:       Yan Naing Aye
/////////////////////////////////////////////////////////////////////////////

#ifndef CEMISC_H
#define CEMISC_H

#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <sstream>
#include <iomanip>
#include "ce/ceMacros.h" // macros

#if defined(CE_WINDOWS)
#include<windows.h>
#include<conio.h> // for kbhit
#else
#include <limits.h>
#include <unistd.h>
#include <sys/ioctl.h> // for kbhit
#include <termios.h> // "
#endif 

namespace ce {
class ceMisc {
private:
public:	
	static std::string alnum(std::string str); // filter the string for alphanumeric characters only
	static std::vector<char> hex2cvec(std::string str); // convert hex string to char vector
	static std::string hex2str(std::string str); // convert hex string to byte string
	static std::string cvec2hex(std::vector<char> bv); // convert char vector to hex string	

	static std::string ToStr16(unsigned int u);// to hexadecimal string
	static std::string ToStr16(int u);// to hexadecimal string
	static std::string ToStr16(unsigned char c);// to hexadecimal string
	static std::string ToStr16(char c);// to hexadecimal string
	static std::string ToStr16(std::vector<char> bv, std::string separator = "", std::string prefix = "", std::string postfix = "");// to hexadecimal string
	static std::string ToStr16(std::vector<unsigned char> bv, std::string separator = "", std::string prefix = "", std::string postfix = "");// to hexadecimal string
	static std::string ToStr16(std::string str, std::string separator = "", std::string prefix = "", std::string postfix = "");// to hexadecimal string
	
	static std::string cvec2str(std::vector<char> bv); // convert char vector to string
	static std::vector<char> cptr2cvec(char* cstr, int n); // char* to char vector	
	static std::string cptr2str(char* cstr, int n); // convert char* to string
	static void cvec2cptr(std::vector<char>& v,char*&cstr, int& n); // character vector to char*
	static std::vector<char> str2cvec(std::string str); // string to char vector
	static std::string f2s(float f, int n = 1); // float to string
	static std::vector<std::string> splitStr(std::string str, std::string delimiter);

	static std::string exepath(); // get the path of executing binary program
	static std::string exedir(); // get the directory of executing binary program

	static bool kb_hit(); // check keyboard hit
	static char get_ch(); // get char
};

} // namespace ce
#endif // CEMISC_H
