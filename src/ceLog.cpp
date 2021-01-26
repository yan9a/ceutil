/////////////////////////////////////////////////////////////////////////////
// Name:         ceLog.cpp
// Description:  logging module 
// Author:       Yan Naing Aye
// Date:         2019 July 22
/////////////////////////////////////////////////////////////////////////////

#include "ce/ceLog.h"
using namespace std;

namespace ce {
ceLog::ceLog()
{
    SetPath(LOG_PATH);
    SetExpiry(30);
    SetExtension(".log");
    SetEnPrintf(false);
}

ceLog::ceLog(string path,double expdays)
{
    SetPath(path);
    SetExpiry(expdays);
    SetExtension(".log");
    SetEnPrintf(true);
}

void ceLog::SetPath(string path)
{
    this->m_path=path;
}

string ceLog::GetPath()
{
    return this->m_path;
}

void ceLog::SetExtension(string ext)
{
    this->m_extension=ext;
}

string ceLog::GetExtension()
{
    return this->m_extension;
}

ceLog::~ceLog()
{

}

bool ceLog::GetEnPrintf()
{
    return this->m_enPrintf;
}

void ceLog::SetEnPrintf(bool en)
{
    this->m_enPrintf = en;
}

double ceLog::GetExpiry()
{
    return this->m_expiry_days;
}

void ceLog::SetExpiry(double days)
{
    this->m_expiry_days=days;
}

void ceLog::SetTimezone(double a)//set time zone
{
    this->m_dt.SetTimezone(a);
}

void ceLog::SetTimezone()//set local time zone
{
    this->m_dt.SetTimezone();
}

double ceLog::GetTimezone()//get time zone
{
    return this->m_dt.tz();
}

int ceLog::Write(string mes)
{
    ofstream wfile;
    int r=-1;
    this->m_dt.Set2Now();
    string logpath=this->m_path+"L"+this->m_dt.ToString("%yyyy-%mm-%dd")+".log";
    try{
        wfile.open(logpath.c_str(),std::fstream::out | std::fstream::app);
        if (wfile.is_open()) {
            wfile << this->m_dt.ToString("%HH:%nn:%ss.%lll") << " " << mes << endl;
            r=0;
        }
        wfile.close();
    }
    catch(...){
        perror("ceLog error in writing");
    }
	return r;
}

int ceLog::Print(string mes)
{
    if(this->m_enPrintf){
        printf("%s\n",mes.c_str());
    }
    return Write(mes);
}

void ceLog::Clean(string path,string extension,double expiry_seconds)
{
    vector<string> filenames;
    string fn="";
    string fpath="";
    double fileage=0;
    int pos=0;

    try{
        ReadDir(path,filenames);
        int nfiles=filenames.size();
        #if ceLog_PRINT == 1
        printf("Number of items: %d \n",nfiles);
        #endif

        if(nfiles<=2){ //if only . and ..
            return;
        }
        this->m_dt.Set2Now();

        for(int i=0;i<nfiles;i++){
            fn=filenames.at(i);
            #if ceLog_PRINT == 1
            printf("File name: %s \n",fn.c_str());
            #endif
            pos=fn.find(extension);
            if(pos>=0){
                fpath=path+fn;
                this->m_ft.SetJD(LastModified(fpath));
                #if ceLog_PRINT ==1
                printf("Modified time: %s \n",m_ft.DateTimeString().c_str());
                #endif
                fileage=(this->m_dt.jd()-this->m_ft.jd())*86400.0;
                if(fileage>expiry_seconds){
                    remove(fpath.c_str());
                    #if ceLog_PRINT == 1
                    printf("%s has been deleted \n", fpath.c_str());
                    //this->Write("Deleted "+fp);
                    #endif
                }
            }
        }
    }
    catch(...){
        perror("ceLog error in cleaning\n");
    }
}
void ceLog::Clean() //clean old log files
{
    this->Clean(this->m_path,this->m_extension,this->m_expiry_days*86400);
}

//http://www.martinbroadhurst.com/list-the-files-in-a-directory-in-c.html
#ifdef CE_WINDOWS
    void ceLog::ReadDir(const string& name, vector<string>& v)
    {
		string pattern(name);
		pattern.append("\\*");
		//WIN32_FIND_DATA data;
		WIN32_FIND_DATAA data;
		HANDLE hFind;

		WCHAR wstr[256];
		//http://msdn.microsoft.com/en-us/library/cc500362.aspx
		MultiByteToWideChar(0, 0, pattern.c_str(), pattern.length(), wstr, 256);
		// LPCSTR lstr = wstr;
		LPBOOL useddefault = FALSE;
		char fn[256];

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
    void ceLog::ReadDir(const string& name, vector<string>& v)
    {
        DIR* dirp = opendir(name.c_str());
        struct dirent * dp;
        while ((dp = readdir(dirp)) != NULL) {
            v.push_back(dp->d_name);
        }
        closedir(dirp);
    }
#endif

double ceLog::LastModified(const string& name)
{
    struct stat attrib;
    struct tm *fmt;
    ceDateTime dt;
    if(stat(name.c_str(), &attrib)==0)
    {
        fmt = localtime(&(attrib.st_mtime));
        dt.SetDateTime(fmt->tm_year+1900,fmt->tm_mon+1,fmt->tm_mday,
            fmt->tm_hour,fmt->tm_min,fmt->tm_sec);
        // printf("Modified time: %s \n",dt.DateTimeString().c_str());
    }
    else{
        // printf("Error in using stat.\n");
    }
    return dt.jd();
}
} // namespace ce 