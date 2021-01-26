/////////////////////////////////////////////////////////////////////////////
// Name:         ceMisc.h
// Description:  utility module
// Author:       Yan Naing Aye
// Date:         2019 July 25
// Last Modified: 2020 October 29
/////////////////////////////////////////////////////////////////////////////

#ifndef CEMISC_H
#define CEMISC_H

#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <sstream>
#include <iomanip>
// #include <wx/wx.h>

namespace ce {
class ceMisc {
private:
public:	
	static std::string alnum(std::string str); // filter the string for alphanumeric characters only
	static std::vector<char> hex2cvec(std::string str); // convert hex string to char vector
	static std::string cvec2hex(std::vector<char> bv); // convert char vector to hex string	
	static std::string cvec2str(std::vector<char> bv); // convert char vector to string
	static std::vector<char> cptr2cvec(char* cstr, int n); // char* to char vector	
	static std::string cptr2str(char* cstr, int n); // convert char* to string
	static void cvec2cptr(std::vector<char>& v,char*&cstr, int& n); // character vector to char*
	static std::vector<char> str2cvec(std::string str); // string to char vector
	static std::string f2s(float f, int n = 1); // float to string
	static std::vector<std::string> splitStr(std::string str, std::string delimiter);

	// wxWidgets
	// static void printTime();
	// static wxImage wx_from_mat(Mat &img);
	// static Mat mat_from_wx(wxImage &wx);
};
} // namespace ce
#endif // CEMISC_H
