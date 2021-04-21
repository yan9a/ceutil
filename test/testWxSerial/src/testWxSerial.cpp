#include <wx/wx.h>
#include "ce/ceUtil.h"
#include <string>
using namespace ce;
#define WXSERIAL_ID1 201
class MyApp : public wxAppConsole
{
  public:
    virtual bool OnInit();
	void OnSerialEvent(wxThreadEvent& event);
	ceLog* _log;
};
IMPLEMENT_APP(MyApp)
bool MyApp::OnInit()
{
	this->_log = new ceLog("./", 1);
	this->_log->SetEnPrintf(true);
	ceWxSerial* com = new ceWxSerial(this,WXSERIAL_ID1,100,"/dev/ttyS6",9600,8,'N',1);
	com->Open();
	Connect(WXSERIAL_ID1, wxEVT_THREAD, wxThreadEventHandler(MyApp::OnSerialEvent));
	char tx[] = "Hello";
	com->Write(tx);
    return true;
}

void MyApp::OnSerialEvent(wxThreadEvent& event)
{
    std::vector<char> v = event.GetPayload<std::vector<char>>();
	this->_log->Print("Ev = "+ ceMisc::cvec2str(v));
	Exit();
}