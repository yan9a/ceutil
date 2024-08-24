/////////////////////////////////////////////////////////////////////////////
// Name:         cewxcvMisc.h
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
using namespace cv;
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

} // namespace ce
#endif // CEWXCVMISC_H
