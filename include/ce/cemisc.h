/////////////////////////////////////////////////////////////////////////////
// Name:         cemisc.h
// Description:  utility module
// Author:       Yan Naing Aye
/////////////////////////////////////////////////////////////////////////////

#ifndef CEMISC_H
#define CEMISC_H

#define CE_DBG_PRINT 0 // print dbg mes

#ifndef CE_MACROS_H
#define CE_MACROS_H

#if defined(_WIN64) || defined(__WIN32__) || defined(_WIN32) || defined(WIN32) || defined(__WINDOWS__) || defined(__TOS_WIN__) || defined(__CYGWIN__)
	#ifndef CE_WINDOWS
		#define CE_WINDOWS
	#endif
#elif defined(__linux__) || defined(unix) || defined(__unix) || defined(__unix__)
	#ifndef CE_LINUX
		#define CE_LINUX
	#endif
#else
	#ifndef CE_NOS
		#define CE_NOS
	#endif
#endif

#if defined(__x86_64) || defined(__x86_64__) || defined(__amd64__) || defined(__amd64)
	#ifndef CE_x86_64
		#define CE_x86_64
	#endif
#elif defined(__arm__) || defined(_M_ARM)
	#ifndef CE_ARM
		#define CE_ARM
	#endif
#else
	#ifndef CE_NARCH
		#define CE_NARCH
	#endif
#endif

#endif // CE_MACROS_H

#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <sstream>
#include <iomanip>

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

/////////////////////////////////////////////////////////////////////////////
// Implementation

// filter the string for alphanumeric characters only 
inline std::string ceMisc::alnum(std::string str)
{
	str.erase(remove_if(str.begin(), str.end(), [](char ch) {
		return !(
			(ch >= '0' && ch <= '9') ||
			(ch >= 'A' && ch <= 'Z') ||
			(ch >= 'a' && ch <= 'z')); }), str.end());
	return str;
}

// convert hex string to char vector
inline std::vector<char> ceMisc::hex2cvec(std::string str)
{
	std::vector<char> v;
	str = ceMisc::alnum(str);// filter for alphanumeric characters
	int n = (int)str.length();
	for (int i = 0; i < n; i += 2)
		v.push_back((char)stoi(str.substr(i, 2), NULL, 16));
	return v;
}

// convert hex string to byte string
inline std::string ceMisc::hex2str(std::string str)
{
	return ceMisc::cvec2str(ceMisc::hex2cvec(str));
}

// convert to hexadecimal string 
inline std::string ceMisc::ToStr16(unsigned int u)
{
	std::ostringstream ss;
	ss << std::setfill('0') << std::setw(2) << std::uppercase << std::hex << u;
	return ss.str();
}

inline std::string ceMisc::ToStr16(int i)
{
	return ceMisc::ToStr16((unsigned int)i);
}

inline std::string ceMisc::ToStr16(unsigned char c)
{
	return ceMisc::ToStr16((unsigned int)c);
}

inline std::string ceMisc::ToStr16(char c)
{
	return ceMisc::ToStr16((unsigned char)c);
}

// convert vector to hexadecimal string
inline std::string ceMisc::ToStr16(std::vector<char> bv, std::string separator, std::string prefix, std::string postfix) {
	std::vector<unsigned char> uv(bv.begin(), bv.end());
	return ceMisc::ToStr16(uv, separator, prefix, postfix);
}

// convert vector to hexadecimal string
inline std::string ceMisc::ToStr16(std::vector<unsigned char> bv, std::string separator, std::string prefix, std::string postfix) {
	std::ostringstream ss;
	unsigned int u;
	for (auto& c : bv) {
		u = (unsigned char)c;
		ss << prefix;
		ss << std::setfill('0') << std::setw(2) << std::uppercase << std::hex << u << postfix;
		if (&c != &bv.back()) ss << separator;
	}
	return ss.str();
}

// to hexadecimal string
inline std::string ceMisc::ToStr16(std::string str, std::string separator, std::string prefix, std::string postfix)
{
	return ceMisc::ToStr16(ceMisc::str2cvec(str), separator, prefix, postfix);
}

// convert char vector to hex string
inline std::string ceMisc::cvec2hex(std::vector<char> bv) {
	std::ostringstream ss;
	unsigned int u;
	for (char& c : bv) {
		u = (unsigned char)c;
		ss << std::setfill('0') << std::setw(2) << std::uppercase << std::hex << u << " ";
	}
	return ss.str();
}

// convert char vector to string
inline std::string ceMisc::cvec2str(std::vector<char> bv) {
	std::ostringstream ss;
	for (char& c : bv) {
		ss << c;
	}
	return ss.str();
}


// character vector to char*
inline void ceMisc::cvec2cptr(std::vector<char>& v, char*& cstr, int& n) {
	n = (int)v.size();
	cstr = v.data(); // reinterpret_cast<char*>(v.data());
}

// char* to char vector
inline std::vector<char> ceMisc::cptr2cvec(char* cstr, int n) {
	std::vector<char> v(cstr, cstr + n);
	return v;
}

// convert char* to string
inline std::string ceMisc::cptr2str(char* cstr, int n) {
	std::ostringstream ss;
	for (int i = 0; i < n; i++) {
		ss << cstr[i];
	}
	return ss.str();
}

// string to char vector
inline std::vector<char> ceMisc::str2cvec(std::string str) {
	std::vector<char> v(str.begin(), str.end());
	return v;
}

inline std::string ceMisc::f2s(float f, int n) {
	std::stringstream ss;
	ss << std::fixed << std::setprecision(n) << f;
	return ss.str();
}

inline std::vector<std::string> ceMisc::splitStr(std::string str, std::string delimiter)
{
	size_t pos = 0;
	std::vector<std::string> tokens;
	while ((pos = str.find(delimiter)) != std::string::npos) {
		tokens.push_back(str.substr(0, pos));
		str.erase(0, pos + delimiter.length());
	}
	tokens.push_back(str);
	return tokens;
}

inline std::string ceMisc::exepath()
{
#ifdef CE_WINDOWS
	char result[MAX_PATH];
	return std::string(result, GetModuleFileNameA(NULL, result, MAX_PATH));
#else
	char result[PATH_MAX];
	ssize_t count = readlink("/proc/self/exe", result, PATH_MAX);
	return std::string(result, (count > 0) ? count : 0);
#endif
}

inline std::string ceMisc::exedir()
{
	std::string str = exepath();
	std::size_t found = str.find_last_of("/\\");
	return str.substr(0, found + 1);
}

inline bool ceMisc::kb_hit() // check keyboard hit
{
#if defined(CE_WINDOWS)
	return _kbhit();
#else
	// https://stackoverflow.com/questions/29335758/using-kbhit-and-getch-on-linux
	termios term;
	tcgetattr(0, &term);

	termios term2 = term;
	term2.c_lflag &= ~ICANON;
	tcsetattr(0, TCSANOW, &term2);

	int byteswaiting;
	ioctl(0, FIONREAD, &byteswaiting);

	tcsetattr(0, TCSANOW, &term);

	return byteswaiting > 0;
#endif 
}

inline char ceMisc::get_ch() // get char
{
#if defined(CE_WINDOWS)
	return _getch();
#else
	//https://stackoverflow.com/questions/421860/capture-characters-from-standard-input-without-waiting-for-enter-to-be-pressed
	char buf = 0;
	struct termios old = { 0 };
	if (tcgetattr(0, &old) < 0)
		perror("tcsetattr()");
	old.c_lflag &= ~ICANON;
	old.c_lflag &= ~ECHO;
	old.c_cc[VMIN] = 1;
	old.c_cc[VTIME] = 0;
	if (tcsetattr(0, TCSANOW, &old) < 0)
		perror("tcsetattr ICANON");
	if (read(0, &buf, 1) < 0)
		perror("read()");
	old.c_lflag |= ICANON;
	old.c_lflag |= ECHO;
	if (tcsetattr(0, TCSADRAIN, &old) < 0)
		perror("tcsetattr ~ICANON");
	return (buf);
#endif
}

/////////////////////////////////////////////////////////////////////////////

} // namespace ce
#endif // CEMISC_H
