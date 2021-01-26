/////////////////////////////////////////////////////////////////////////////
// Name:          ceWxTmr.h
// Description:   Timer module
// Author:        Yan Naing Aye
// Date:          2019 September 10
// Last modified: 2020 October 27
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
