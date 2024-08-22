#include <wx/wx.h>
#include "ce/ceMacros.h"
#include "ce/ceMisc.h"
#include <string>
using namespace std;
using namespace ce;
#if defined(CE_WINDOWS)
int main()
{
	cout << "Press anykey to test kbhit!" << endl;
	while (1) {
		if (ceMisc::kb_hit()) {
			char c = ceMisc::get_ch();
			std::cout << ("Key pressed: " + std::to_string(c)) << std::endl;
		}
	}
	return 0;
}
#else
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
	t->Start(10);
	Connect(TMR_ID1, wxEVT_TIMER, wxTimerEventHandler(MyApp::OnTmrEvent));
    return true;
}

void MyApp::OnTmrEvent(wxTimerEvent& WXUNUSED(event))
{
	if (ceMisc::kb_hit()) {
		char c = ceMisc::get_ch();
		std::cout << ("Key pressed: " + std::to_string(c)) << std::endl;
	}
}
#endif
