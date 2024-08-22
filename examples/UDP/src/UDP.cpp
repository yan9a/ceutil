#include "ce/ceUtil.h"
using namespace ce;
#define CE_TMR_ID1 202
#define CE_UDP_ID1 203
class MyApp : public wxAppConsole
{
private:
	wxTimer* _tmr1;
	ceUDP* _udp;
public:
	virtual bool OnInit();
	void OnTimer1(wxTimerEvent& WXUNUSED(event));
	void OnUdpEvent(wxThreadEvent& event);
};
IMPLEMENT_APP(MyApp)
bool MyApp::OnInit()
{
	this->_tmr1 = new wxTimer(this,CE_TMR_ID1);
	this->_udp = new ceUDP(this,CE_UDP_ID1,10101,10102);
	this->_udp->Open();
	Connect(CE_TMR_ID1, wxEVT_TIMER, wxTimerEventHandler(MyApp::OnTimer1));
	Connect(CE_UDP_ID1, wxEVT_THREAD, wxThreadEventHandler(MyApp::OnUdpEvent));
	this->_tmr1->Start(3000);
    return true;
}

void MyApp::OnTimer1(wxTimerEvent& WXUNUSED(event))
{
	static int c = 0;
	printf("Tmr ev...\n");
	std::vector<char> vc = {'U','D','P'};
	this->_udp->Tx(vc);
	if(++c>=3){
		Exit();
	}
}

void MyApp::OnUdpEvent(wxThreadEvent& event)
{
	std::vector<char> v = event.GetPayload<std::vector<char>>();
	std::string s = event.GetString().ToStdString();
	int i = event.GetInt();
	printf("UDP vec = %s, ip =%s, port = %d\n",ceMisc::cvec2hex(v).c_str(),s.c_str(),i);
}
