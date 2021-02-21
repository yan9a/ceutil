/////////////////////////////////////////////////////////////////////////////
// Name:         ceConfig.cpp
// Description:  Configuration module 
// Author:       Yan Naing Aye
/////////////////////////////////////////////////////////////////////////////

#include "ce/ceConfig.h"
using namespace std;

namespace ce {
ceConfig::ceConfig() : m_path(CONF_PATH)
{

}

ceConfig::ceConfig(string path) : m_path(path)
{

}

ceConfig::~ceConfig()
{

}

Json::Value ceConfig::GetJson()
{
	return ReadJson(this->m_path);
}

Json::Value ceConfig::ReadJson(std::string path)
{
	string document = ceConfig::ReadFile(path);
	return ceConfig::GetJson(document);
}

Json::Value ceConfig::GetJson(std::string jstr)
{
	stringstream ss;
	Json::CharReaderBuilder reader;
	Json::Value obj;
	string errs;
	ss.str(jstr);
	if (Json::parseFromStream(reader, ss, &obj, &errs)) {
		//printf("Parsing json string OK\n");
	}
	else {
		perror("Error in json parsing\n");
	}
	return obj;
}

string ceConfig::ReadFile(string path)
{
    string sstr = "", tstr = "";
    ifstream mys;
	try{
		mys.open(path.c_str());
		if (mys.is_open()) {
			while (getline(mys, tstr)) {
				sstr += tstr;
			}
			mys.close();
		}
	}
	catch(...){
		perror("Error in reading json file\n");
	}
    return sstr;
}

int ceConfig::WriteFile(string path,string str)
{
    ofstream wfile;
    int r=-1;
	try{
		wfile.open(path.c_str());
		if (wfile.is_open()) {
			wfile << str;
			r=0;
		}
		wfile.close();
	}
	catch(...){
		perror("Error in json file writing\n");
	}
    return r;
}

int ceConfig::WriteJson(Json::Value obj, std::string path)
{
	string str = ceConfig::ToString(obj);
	return ceConfig::WriteFile(path,str);
}

std::string ceConfig::ToString(Json::Value obj)
{
	stringstream ss;
	ss << obj;
	return ss.str();
}

int ceConfig::SaveJson(Json::Value obj)
{
	return ceConfig::WriteJson(obj, this->m_path);
}

} // namespace ce 

