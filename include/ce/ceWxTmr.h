/////////////////////////////////////////////////////////////////////////////
// Name:          ceWxTmr.h
// Description:   Timer module
// Author:        Yan Naing Aye
/////////////////////////////////////////////////////////////////////////////

#ifndef CEWXTMR_H
#define CEWXTMR_H

#include "wx/wx.h"
namespace ce {

class ceWxTmr : public wxEvtHandler {
public:
	ceWxTmr(wxAppConsole* app,int id, int interval);
	void OnTimer(wxTimerEvent& event);
	void Start();
private:
	wxTimer* _timer;
	wxAppConsole* _app;
	int _id;
	int _interval;
};

} // namespace ce 

#endif // CEWXTMR_H
