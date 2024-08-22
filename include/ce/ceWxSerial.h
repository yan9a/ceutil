/////////////////////////////////////////////////////////////////////////////
// Name:          ceWxSerial.h
// Description:   Serial module for wxWidgets
// Author:        Yan Naing Aye
/////////////////////////////////////////////////////////////////////////////

#ifndef CEWXSERIAL_H
#define CEWXSERIAL_H
#include "ce/ceMacros.h" // macros
#if CE_WX==1
#include "wx/wx.h"
#include <vector>
#include "ce/ceSerial.h"
namespace ce {

class ceWxSerial : public wxEvtHandler, public ce::ceSerial {
public:
	ceWxSerial(wxEvtHandler* app,int id, int interval, 
		std::string Device, long BaudRate, long DataSize, char ParityType, float NStopBits);
	~ceWxSerial();
	void OnTimer(wxTimerEvent& event);
	std::vector<char> Chk();
private:
	wxTimer* _timer;
	wxEvtHandler* _app;
	int _id;
	int _interval;
};

} // namespace ce 
#endif // CE_WX
#endif // CEWXSERIAL_H
