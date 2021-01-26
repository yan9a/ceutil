/////////////////////////////////////////////////////////////////////////////
// Name:         ceConfig.cpp
// Description:  Configuration module 
// Author:       Yan Naing Aye
// Date:         2019 July 22
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
	stringstream ss;	
	Json::CharReaderBuilder reader;
	Json::Value obj;
	string errs;
	string document;
	document = ReadFile(m_path);
	ss.str(document);
	if (Json::parseFromStream(reader, ss, &obj, &errs)) {
		printf("ceConfig parsing %s OK\n",m_path.c_str());
	}
	else {
		perror("ceConfig error in parsing\n");
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
		perror("ceConfig error in reading\n");
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
		perror("ceConfig error in writing\n");
	}
    return r;
}

} // namespace ce 

