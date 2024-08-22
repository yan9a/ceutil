 #include <wx/wx.h>
#include "ce/ceMacros.h"
#include "ce/ceMisc.h"
#include <string>
using namespace std;
using namespace ce;

#define TMR_ID1 201
class MyApp : public wxAppConsole
{
  public:
    virtual bool OnInit();
	void OnTmrEvent(wxTimerEvent& WXUNUSED(event));
};
IMPLEMENT_APP(MyApp)
bool MyApp::OnInit()
{
	wxTimer* t = new wxTimer(this,TMR_ID1);
	t->Start(1000);
	Connect(TMR_ID1, wxEVT_TIMER, wxTimerEventHandler(MyApp::OnTmrEvent));
    return true;
}

void MyApp::OnTmrEvent(wxTimerEvent& WXUNUSED(event))
{
	std::cout << "tmr ev" << std::endl;

}

