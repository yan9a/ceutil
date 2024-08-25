/////////////////////////////////////////////////////////////////////////////
// Name:         cewxcvmisc.h
// Description:  utility module using wxWidgets and OpenCV
// Author:       Yan Naing Aye
/////////////////////////////////////////////////////////////////////////////

#ifndef CEWXCVMISC_H
#define CEWXCVMISC_H

#ifndef CE_MACROS_H
#define CE_MACROS_H

#define CE_WX 1 // include WX
#define CE_CV 1 // include CV
#define CE_DBG_PRINT 0 // print dbg mes

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

#include <string>
#if CE_WX==1
#include "wx/wx.h"
#endif // CE_WX
#if CE_CV==1
#include <opencv2/opencv.hpp>
#endif // CE_WX
namespace ce {
class cewxcvMisc {
private:
public:	
#if CE_WX==1
	// wxWidgets
	 static void printTime();
#if CE_CV==1
	 static wxImage wx_from_mat(cv::Mat &img);
	 static cv::Mat mat_from_wx(wxImage &wx);
#endif // CE_CV
#endif // CE_WX
};

/////////////////////////////////////////////////////////////////////////////
// Implementation
#if CE_WX==1
inline void cewxcvMisc::printTime()
{
	wxDateTime* wdt;
	wdt = new wxDateTime();
	wdt->SetToCurrent();
	wxString wstr = wdt->Format(wxT("%Y-%m-%d %H:%M:%S"), wxDateTime::Local);
	// wxPuts(wstr);
	std::string str = wstr.ToStdString();
	printf("%s\n", str.c_str());
}
#if CE_CV==1
inline wxImage cewxcvMisc::wx_from_mat(cv::Mat& img) {
	cv::Mat im2;
	if (img.channels() == 1) { cvtColor(img, im2, cv::COLOR_GRAY2RGB); }
	else if (img.channels() == 4) { cvtColor(img, im2, cv::COLOR_BGRA2RGB); }
	else { cvtColor(img, im2, cv::COLOR_BGR2RGB); }
	long imsize = im2.rows * im2.cols * im2.channels();
	wxImage wx(im2.cols, im2.rows, (unsigned char*)malloc(imsize), false);
	unsigned char* s = im2.data;
	unsigned char* d = wx.GetData();
	for (long i = 0; i < imsize; i++) { d[i] = s[i]; }
	return wx;
}

inline cv::Mat cewxcvMisc::mat_from_wx(wxImage& wx) {
	cv::Mat im2(cv::Size(wx.GetWidth(), wx.GetHeight()), CV_8UC3, wx.GetData());
	cvtColor(im2, im2, cv::COLOR_RGB2BGR);
	return im2;
}
#endif // CE_CV
#endif // CE_WX
/////////////////////////////////////////////////////////////////////////////

} // namespace ce
#endif // CEWXCVMISC_H
