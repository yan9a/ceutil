/////////////////////////////////////////////////////////////////////////////
// Name:         ceWxSerial.cpp
// Description:   Serial module for wxWidgets
// Author:        Yan Naing Aye
/////////////////////////////////////////////////////////////////////////////
#include "ce/ceWxSerial.h"
#if CE_WX==1

using namespace std;

namespace ce {

//ctor
ceWxSerial::ceWxSerial(wxEvtHandler* app,int id, int interval, 
	std::string Device, long BaudRate, long DataSize, char ParityType, float NStopBits)
	: _app(app), _id(id), _interval(interval), ceSerial(Device, BaudRate, DataSize, ParityType, NStopBits)
{
	this->_timer = new wxTimer(this, _id);
	Connect(_id, wxEVT_TIMER, wxTimerEventHandler(ceWxSerial::OnTimer));		
	this->_timer->Start(_interval - 1);
}

ceWxSerial::~ceWxSerial()
{

}

void ceWxSerial::OnTimer(wxTimerEvent& WXUNUSED(event))
{
	vector<char> vc = this->Chk();
	if (vc.size() > 0) {
		wxThreadEvent event(wxEVT_THREAD, _id);
		event.SetPayload(vc);
		// send in a thread-safe way
		wxQueueEvent(_app, event.Clone());
	}
}

vector<char> ceWxSerial::Chk()
{
	vector<char> vc;
	char ch; bool r;
	do {
		ch = this->ReadChar(r);
		if (r) {
			vc.push_back(ch);
		}
	} while (r);
	return vc;
}

} // namespace ce 
#endif // CE_WX
