/////////////////////////////////////////////////////////////////////////////
// Name:         ceConfig.h
// Description:  Configuration module for Dagger
// Author:       Yan Naing Aye
// Date:         2020 Nov 4
/////////////////////////////////////////////////////////////////////////////

#ifndef CECONFIG_H
#define CECONFIG_H

#if defined(_WIN64) || defined(__WIN32__) || defined(_WIN32) || defined(WIN32) || defined(__WINDOWS__) || defined(__TOS_WIN__)
    #define CE_WINDOWS
    #include <windows.h>
    #define stat _stat
#else
    #define CE_LINUX
    #include <unistd.h>
    #include <sys/types.h>
    #include <dirent.h>
    #include <sys/stat.h>
    #include <sys/time.h>
#endif // defined

#include <stdio.h>
#include <cstdint>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <json/json.h>

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
	static T Validate(T value,T min_val,T max_val,T default_val);		
	Json::Value GetJson();
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

} // namespace ce 
#endif // CECONFIG_H
