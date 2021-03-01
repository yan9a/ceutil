/////////////////////////////////////////////////////////////////////////////
// Name:         cewxcvMisc.cpp
// Description:  utility module using wxWidgets and OpenCV
// Author:       Yan Naing Aye
/////////////////////////////////////////////////////////////////////////////
#include "ce/cewxcvMisc.h"
using namespace std;
#if CE_CV==1
using namespace cv;
#endif // CE_WX
namespace ce {

#if CE_WX==1
 void cewxcvMisc::printTime()
 {
 	wxDateTime* wdt;
 	wdt = new wxDateTime();
 	wdt->SetToCurrent();
 	wxString wstr = wdt->Format(wxT("%Y-%m-%d %H:%M:%S"), wxDateTime::Local);
 	// wxPuts(wstr);
 	std::string str = wstr.ToStdString();
 	printf("%s\n",str.c_str());
 }
#if CE_CV==1
 wxImage cewxcvMisc::wx_from_mat(Mat &img) {
     Mat im2;
     if(img.channels()==1){cvtColor(img,im2,COLOR_GRAY2RGB);}
 	else if (img.channels() == 4) { cvtColor(img, im2, COLOR_BGRA2RGB);}
     else {cvtColor(img,im2,COLOR_BGR2RGB);}
 	long imsize = im2.rows*im2.cols*im2.channels();
     wxImage wx(im2.cols, im2.rows,(unsigned char*)malloc(imsize), false);
 	unsigned char* s=im2.data;
 	unsigned char* d=wx.GetData();
 	for (long i = 0; i < imsize; i++) { d[i] = s[i];}
     return wx;
 }

 Mat cewxcvMisc::mat_from_wx(wxImage &wx) {
     Mat im2(Size(wx.GetWidth(),wx.GetHeight()),CV_8UC3,wx.GetData());
     cvtColor(im2,im2,COLOR_RGB2BGR);
     return im2;
 }
#endif // CE_CV
#endif // CE_WX
} // namespace ce