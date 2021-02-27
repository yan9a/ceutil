/////////////////////////////////////////////////////////////////////////////
// Name:         ceConfig.h
// Description:  Configuration
// Author:       Yan Naing Aye
/////////////////////////////////////////////////////////////////////////////

#ifndef CECONFIG_H
#define CECONFIG_H

#include "ceMacros.h" // macros

#include <stdio.h>
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

} // namespace ce 
#endif // CECONFIG_H
