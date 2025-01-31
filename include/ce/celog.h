/////////////////////////////////////////////////////////////////////////////
// Name:         celog.h
// Description:  logging module 
// Author:       Yan Naing Aye
/////////////////////////////////////////////////////////////////////////////

#ifndef ceLog_H
#define ceLog_H

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
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include "ce/cedt.h"

#if defined(CE_WINDOWS)
    #include <windows.h>
    #define stat _stat
#else
    #include <unistd.h>
    #include <sys/types.h>
    #include <dirent.h>
    #include <sys/stat.h>
#endif

#define LOG_PATH "./log/"
#define CELOG_DBG_PRINT 0 // print dbg mes

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

/////////////////////////////////////////////////////////////////////////////
// Implementation

inline ceLog::ceLog()
{
    SetPath(LOG_PATH);
    SetExpiry(30);
    SetExtension(".log");
    SetEnPrintf(false);
}

inline ceLog::ceLog(std::string path, double expdays)
{
    SetPath(path);
    SetExpiry(expdays);
    SetExtension(".log");
    SetEnPrintf(true);
}

inline void ceLog::SetPath(std::string path)
{
    this->m_path = path;
}

inline std::string ceLog::GetPath()
{
    return this->m_path;
}

inline void ceLog::SetExtension(std::string ext)
{
    this->m_extension = ext;
}

inline std::string ceLog::GetExtension()
{
    return this->m_extension;
}

inline ceLog::~ceLog()
{

}

inline bool ceLog::GetEnPrintf()
{
    return this->m_enPrintf;
}

inline void ceLog::SetEnPrintf(bool en)
{
    this->m_enPrintf = en;
}

inline double ceLog::GetExpiry()
{
    return this->m_expiry_days;
}

inline void ceLog::SetExpiry(double days)
{
    this->m_expiry_days = days;
}

inline void ceLog::SetTimezone(double a)//set time zone
{
    this->m_dt.SetTimezone(a);
}

inline void ceLog::SetTimezone()//set local time zone
{
    this->m_dt.SetTimezone();
}

inline double ceLog::GetTimezone()//get time zone
{
    return this->m_dt.tz();
}

inline int ceLog::Write(std::string mes)
{
    std::ofstream wfile;
    int r = -1;
    this->m_dt.Set2Now();
    std::string logpath = this->m_path + "L" + this->m_dt.ToString("%yyyy-%mm-%dd") + ".log";
    try {
        wfile.open(logpath.c_str(), std::fstream::out | std::fstream::app);
        if (wfile.is_open()) {
            wfile << this->m_dt.ToString("%HH:%nn:%ss.%lll") << " " << mes << std::endl;
            r = 0;
        }
        wfile.close();
    }
    catch (...) {
        perror("ceLog error in writing");
    }
    return r;
}

inline int ceLog::Print(std::string mes)
{
    if (this->m_enPrintf) {
        // printf("%s\n",mes.c_str());
        std::cout << mes << std::endl;
    }
    return Write(mes);
}

inline void ceLog::Clean(std::string path, std::string extension, double expiry_seconds)
{
    std::vector<std::string> filenames;
    std::string fn = "";
    std::string fpath = "";
    double fileage = 0;
    int pos = 0;

    try {
        ReadDir(path, filenames);
        int nfiles = (int)filenames.size();
#if CELOG_DBG_PRINT == 1
        printf("Number of items: %d \n", nfiles);
#endif

        if (nfiles <= 2) { //if only . and ..
            return;
        }
        this->m_dt.Set2Now();

        for (int i = 0; i < nfiles; i++) {
            fn = filenames.at(i);
#if CELOG_DBG_PRINT == 1
            printf("File name: %s \n", fn.c_str());
#endif
            pos = (int)fn.find(extension);
            if (pos >= 0) {
                fpath = path + fn;
                this->m_ft.SetJD(LastModified(fpath));
#if CELOG_DBG_PRINT ==1
                printf("Modified time: %s \n", m_ft.ToString("%yyyy-%mm-%dd %HH:%nn:%ss").c_str());
#endif
                fileage = (this->m_dt.jd() - this->m_ft.jd()) * 86400.0;
                if (fileage > expiry_seconds) {
                    remove(fpath.c_str());
#if CELOG_DBG_PRINT == 1
                    printf("%s has been deleted \n", fpath.c_str());
                    //this->Write("Deleted "+fp);
#endif
                }
            }
        }
    }
    catch (...) {
        perror("ceLog error in cleaning\n");
    }
}
inline void ceLog::Clean() //clean old log files
{
    this->Clean(this->m_path, this->m_extension, this->m_expiry_days * 86400);
}

//http://www.martinbroadhurst.com/list-the-files-in-a-directory-in-c.html
#ifdef CE_WINDOWS
inline void ceLog::ReadDir(const std::string& name, std::vector<std::string>& v)
{
    std::string pattern(name);
    pattern.append("\\*");
    //WIN32_FIND_DATA data;
    WIN32_FIND_DATAA data;
    HANDLE hFind;

    WCHAR wstr[256];
    //http://msdn.microsoft.com/en-us/library/cc500362.aspx
    MultiByteToWideChar(0, 0, pattern.c_str(), (int)pattern.length(), wstr, 256);
    // LPCSTR lstr = wstr;
    LPBOOL useddefault = FALSE;
    // char fn[256];

    if ((hFind = FindFirstFileA(pattern.c_str(), &data)) != INVALID_HANDLE_VALUE) {
        //if ((hFind = FindFirstFile(wstr, &data)) != INVALID_HANDLE_VALUE) {
        do {
            //WideCharToMultiByte(0, 0, data.cFileName, -1, fn, 256, NULL, useddefault);
            //v.push_back(string(fn));
            v.push_back(data.cFileName);
        } while (FindNextFileA(hFind, &data) != 0);
        FindClose(hFind);
    }
}
#else
inline void ceLog::ReadDir(const std::string& name, std::vector<std::string>& v)
{
    DIR* dirp = opendir(name.c_str());
    struct dirent* dp;
    while ((dp = readdir(dirp)) != NULL) {
        v.push_back(dp->d_name);
    }
    closedir(dirp);
}
#endif

inline double ceLog::LastModified(const std::string& name)
{
    struct stat attrib;
    struct tm* fmt;
    ceDateTime dt;
    if (stat(name.c_str(), &attrib) == 0)
    {
        fmt = localtime(&(attrib.st_mtime));
        dt.SetDateTime(fmt->tm_year + 1900, fmt->tm_mon + 1, fmt->tm_mday,
            fmt->tm_hour, fmt->tm_min, fmt->tm_sec,dt.tz());
#if CELOG_DBG_PRINT == 1
        printf("Year: %d\n", (fmt->tm_year + 1900));
        printf("Month: %d\n", (fmt->tm_mon+1));
        printf("MDay: %d\n", fmt->tm_mday);
        printf("Hour: %d\n", fmt->tm_hour);
        printf("Min: %d\n", fmt->tm_min);
        printf("Sec: %d\n", fmt->tm_sec );
        printf("Modified time: %s \n",dt.ToString("%yyyy-%mm-%dd %HH:%nn:%ss").c_str());
#endif
    }
    else {
        // printf("Error in using stat.\n");
    }
    return dt.jd();
}

/////////////////////////////////////////////////////////////////////////////

} // namespace ce 


#endif
