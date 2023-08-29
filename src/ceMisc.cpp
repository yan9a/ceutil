/////////////////////////////////////////////////////////////////////////////
// Name:         ceMisc.cpp
// Description:  utility module
// Author:       Yan Naing Aye
/////////////////////////////////////////////////////////////////////////////
#include "ce/ceMisc.h"
using namespace std;
namespace ce {

// filter the string for alphanumeric characters only 
string ceMisc::alnum(string str)
{
	str.erase(remove_if(str.begin(), str.end(), [](char ch) {
		return !(
			(ch >= '0' && ch <= '9') ||
			(ch >= 'A' && ch <= 'Z') ||
			(ch >= 'a' && ch <= 'z')); }), str.end());
	return str;
}

// convert hex string to char vector
vector<char> ceMisc::hex2cvec(string str)
{
	vector<char> v;
	str = ceMisc::alnum(str);// filter for alphanumeric characters
	int n = (int)str.length();
	for (int i = 0; i < n; i += 2) 
		v.push_back((char)stoi(str.substr(i, 2), NULL, 16));
	return v;
}

// convert hex string to byte string
std::string ceMisc::hex2str(std::string str)
{
	return ceMisc::cvec2str(ceMisc::hex2cvec(str));
}

// convert to hexadecimal string 
std::string ceMisc::ToStr16(unsigned int u)
{
	ostringstream ss;
	ss << setfill('0') << setw(2) << uppercase << hex << u;
	return ss.str();
}

std::string ceMisc::ToStr16(int i)
{
	return ceMisc::ToStr16((unsigned int)i);
}

std::string ceMisc::ToStr16(unsigned char c)
{
	return ceMisc::ToStr16((unsigned int)c);
}

std::string ceMisc::ToStr16(char c)
{
	return ceMisc::ToStr16((unsigned char)c);
}

// convert vector to hexadecimal string
std::string ceMisc::ToStr16(std::vector<char> bv, std::string separator, std::string prefix, std::string postfix) {
	std::vector<unsigned char> uv(bv.begin(), bv.end());
	return ceMisc::ToStr16(uv,separator,prefix,postfix);
}

// convert vector to hexadecimal string
std::string ceMisc::ToStr16(std::vector<unsigned char> bv, std::string separator, std::string prefix, std::string postfix) {
	ostringstream ss;
	unsigned int u;
	for (auto& c : bv) {
		u = (unsigned char)c;
		ss << prefix;
		ss << setfill('0') << setw(2) << uppercase << hex << u << postfix;
		if (&c != &bv.back()) ss << separator;
	}
	return ss.str();
}

// to hexadecimal string
std::string ceMisc::ToStr16(std::string str, std::string separator, std::string prefix, std::string postfix)
{
	return ceMisc::ToStr16(ceMisc::str2cvec(str), separator, prefix, postfix);
}

// convert char vector to hex string
string ceMisc::cvec2hex(vector<char> bv) {
	ostringstream ss;
	unsigned int u;
	for (char& c : bv) {
		u = (unsigned char)c;
		ss << setfill('0') << setw(2) << uppercase << hex << u << " ";
	}
	return ss.str();
}

// convert char vector to string
string ceMisc::cvec2str(vector<char> bv) {
	ostringstream ss;
	for (char& c : bv) {
		ss << c;
	}
	return ss.str();
}


// character vector to char*
void ceMisc::cvec2cptr(vector<char>& v, char*& cstr, int& n) {
	n = (int)v.size();
	cstr = v.data(); // reinterpret_cast<char*>(v.data());
}

// char* to char vector
vector<char> ceMisc::cptr2cvec(char* cstr,int n) {
	vector<char> v(cstr, cstr + n);
	return v;
}

// convert char* to string
string ceMisc::cptr2str(char* cstr, int n) {
	ostringstream ss;
	for (int i = 0; i < n;i++) {
		ss << cstr[i];
	}
	return ss.str();
}

// string to char vector
vector<char> ceMisc::str2cvec(string str) {
	vector<char> v(str.begin(), str.end());
	return v;
}

string ceMisc::f2s(float f, int n) {
	stringstream ss;
	ss<<fixed<<setprecision(n)<<f;
	return ss.str();
}

vector<string> ceMisc::splitStr(string str, string delimiter)
{
	size_t pos = 0;
	vector<string> tokens;
	while ((pos = str.find(delimiter)) != string::npos) {
    		tokens.push_back(str.substr(0, pos));
		str.erase(0, pos + delimiter.length());
	}
	tokens.push_back(str);
	return tokens;
}

std::string ceMisc::exepath()
{
#ifdef CE_WINDOWS
	char result[MAX_PATH];
	return std::string(result, GetModuleFileName(NULL, result, MAX_PATH));
#else
	char result[PATH_MAX];
	ssize_t count = readlink("/proc/self/exe", result, PATH_MAX);
	return std::string(result, (count > 0) ? count : 0);
#endif
}

std::string ceMisc::exedir()
{
	std::string str = exepath();
	std::size_t found = str.find_last_of("/\\");
	return str.substr(0, found + 1);
}

bool ceMisc::kb_hit() // check keyboard hit
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

char ceMisc::get_ch() // get char
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

} // namespace ce