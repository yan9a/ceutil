/////////////////////////////////////////////////////////////////////////////
// Name:         ceLog.h
// Description:  logging module 
// Author:       Yan Naing Aye
// Date:         2019 July 22
/////////////////////////////////////////////////////////////////////////////

#ifndef ceLog_H
#define ceLog_H

#include <stdio.h>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include "ceDateTime.h"

#if defined(_WIN64) || defined(__WIN32__) || defined(_WIN32) || defined(WIN32) || defined(__WINDOWS__) || defined(__TOS_WIN__)
    #ifndef CE_WINDOWS
        #define CE_WINDOWS
    #endif
    #include <windows.h>
    #define stat _stat
    // using namespace System;
#else
    #ifndef CE_LINUX
        #define CE_LINUX
    #endif
    #include <unistd.h>
    #include <sys/types.h>
    #include <dirent.h>
    #include <sys/stat.h>
#endif

#define ceLog_PRINT 0
#define LOG_PATH "./log/"

namespace ce {
class ceLog{
	std::string m_path;
	ceDateTime m_dt;
	ceDateTime m_ft;
	double m_expiry_days;
	std::string m_extension;
	bool m_enPrintf;
public:
    ceLog();
	ceLog(std::string path,double expdays);
	~ceLog();
	void SetPath(std::string path);
	std::string GetPath();
	void SetExtension(std::string ext);
	std::string GetExtension();
	void SetExpiry(double days);
	double GetExpiry();
	int Write(std::string mes); // write to file only
	void Clean();//clean old log files
	void Clean(std::string path,std::string extension,double expiry_seconds);
	void SetTimezone(double a);
	void SetTimezone();//local time
	double GetTimezone();
	void ReadDir(const std::string& name, std::vector<std::string>& v);
	double LastModified(const std::string& name);//return julian date
	int Print(std::string mes); // write to file and echo on std output
	bool GetEnPrintf();
	void SetEnPrintf(bool en);
};
} // namespace ce 


#endif
