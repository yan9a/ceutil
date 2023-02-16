#include <wx/wx.h>
#include "ce/ceUtil.h"
#include <string>
using namespace ce;
#define TMR_ID1 201
class MyApp : public wxAppConsole
{
  public:
    virtual bool OnInit();
	void OnTmrEvent(wxThreadEvent& event);
	ceLog* _log;
};
IMPLEMENT_APP(MyApp)
bool MyApp::OnInit()
{
	this->_log = new ceLog("./", 1);
	this->_log->SetEnPrintf(true);
	ceWxTmr* t = new ceWxTmr(this,TMR_ID1,2000);// 2 seconds
	t->Start();
	Connect(TMR_ID1, wxEVT_THREAD, wxThreadEventHandler(MyApp::OnTmrEvent));
    return true;
}

void MyApp::OnTmrEvent(wxThreadEvent& event)
{
	static int c = 0;
    std::vector<char> v = event.GetPayload<std::vector<char>>();
	this->_log->Print("Ev = "+ std::to_string(int(v[0])));
	if(++c >= 3){
		Exit();
	}
}