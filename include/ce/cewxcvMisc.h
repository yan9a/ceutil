/////////////////////////////////////////////////////////////////////////////
// Name:         cewxcvMisc.h
// Description:  utility module using wxWidgets and OpenCV
// Author:       Yan Naing Aye
/////////////////////////////////////////////////////////////////////////////

#ifndef CEWXCVMISC_H
#define CEWXCVMISC_H

#include <string>
#include "ceMacros.h" // macros
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
