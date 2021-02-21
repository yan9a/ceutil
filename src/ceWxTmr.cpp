/////////////////////////////////////////////////////////////////////////////
// Name:         ceWxTmr.cpp
// Description:  Timer module
// Author:       Yan Naing Aye
/////////////////////////////////////////////////////////////////////////////

#include "ce/ceWxTmr.h"
#include<vector>
using namespace std;
namespace ce {
//ctor
ceWxTmr::ceWxTmr(wxAppConsole* app,int id, int interval) : _app(app), _id(id), _interval(interval)
{
	this->_timer = new wxTimer(this, _id);
	Connect(_id, wxEVT_TIMER, wxTimerEventHandler(ceWxTmr::OnTimer));		
}

void ceWxTmr::OnTimer(wxTimerEvent& WXUNUSED(event))
{
	printf("ceWxTmr event...\n");
	vector<char> vc = {64,65};
	wxThreadEvent event( wxEVT_THREAD, _id );
	event.SetPayload(vc);
	// send in a thread-safe way
	wxQueueEvent( _app, event.Clone() );
}

void ceWxTmr::Start()
{
	this->_timer->Start(_interval-1);
}

} // namespace ce 