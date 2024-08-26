/////////////////////////////////////////////////////////////////////////////
// Name:         ceconf.h
// Description:  Configuration
// Author:       Yan Naing Aye
/////////////////////////////////////////////////////////////////////////////
// Requires: jsoncpp
/////////////////////////////////////////////////////////////////////////////

#ifndef CECONFIG_H
#define CECONFIG_H

#define CE_JSON 1 // include JSON

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

#include <cstdio>
#include <cstdint>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#if CE_JSON==1
#include <json/json.h>
#endif

#define CONF_PATH "./conf.json"
namespace ce {
class ceConfig {
public:
	ceConfig();
	ceConfig(std::string path);
	virtual ~ceConfig();
	static std::string ReadFile(std::string path);
	static int WriteFile(std::string path,std::string str);
	template <typename T>
	static T Validate(T value,T min_val,T max_val,T default_val);	// defined in header	
#if CE_JSON==1
	Json::Value GetJson();
	static Json::Value GetJson(std::string jstr);
	static Json::Value ReadJson(std::string path);
	static int WriteJson(Json::Value obj, std::string path);
	static std::string ToString(Json::Value obj);
	int SaveJson(Json::Value obj);
#endif
private:
	std::string m_path;
};

//example usage
// int i=Validate<int>(v,1,10,5);
template <typename T>
T ceConfig::Validate(T value,T min_val,T max_val,T default_val)
{
    if(value<min_val || value>max_val) value=default_val;
    return value;
}


/////////////////////////////////////////////////////////////////////////////
// Implementation

inline ceConfig::ceConfig() : m_path(CONF_PATH)
{

}

inline ceConfig::ceConfig(std::string path) : m_path(path)
{

}

inline ceConfig::~ceConfig()
{

}

inline std::string ceConfig::ReadFile(std::string path)
{
	std::string sstr = "", tstr = "";
	std::ifstream mys;
	try {
		mys.open(path.c_str());
		if (mys.is_open()) {
			while (getline(mys, tstr)) {
				sstr += tstr;
			}
			mys.close();
		}
	}
	catch (...) {
		perror("Error in reading json file\n");
	}
	return sstr;
}

inline int ceConfig::WriteFile(std::string path, std::string str)
{
	std::ofstream wfile;
	int r = -1;
	try {
		wfile.open(path.c_str());
		if (wfile.is_open()) {
			wfile << str;
			r = 0;
		}
		wfile.close();
	}
	catch (...) {
		perror("Error in json file writing\n");
	}
	return r;
}

#if CE_JSON==1
inline Json::Value ceConfig::GetJson()
{
	return ReadJson(this->m_path);
}

inline Json::Value ceConfig::ReadJson(std::string path)
{
	std::string document = ceConfig::ReadFile(path);
	return ceConfig::GetJson(document);
}

inline Json::Value ceConfig::GetJson(std::string jstr)
{
	std::stringstream ss;
	Json::CharReaderBuilder reader;
	Json::Value obj;
	std::string errs;
	ss.str(jstr);
	if (Json::parseFromStream(reader, ss, &obj, &errs)) {
		//printf("Parsing json string OK\n");
	}
	else {
		perror("Error in json parsing\n");
	}
	return obj;
}

inline int ceConfig::WriteJson(Json::Value obj, std::string path)
{
	std::string str = ceConfig::ToString(obj);
	return ceConfig::WriteFile(path, str);
}

inline std::string ceConfig::ToString(Json::Value obj)
{
	std::stringstream ss;
	ss << obj;
	return ss.str();
}

inline int ceConfig::SaveJson(Json::Value obj)
{
	return ceConfig::WriteJson(obj, this->m_path);
}
#endif // if CE_JSON
/////////////////////////////////////////////////////////////////////////////

} // namespace ce 
#endif // CECONFIG_H
