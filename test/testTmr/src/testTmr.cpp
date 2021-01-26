#include "ce/ceUtil.h"
using namespace ce;
#define TMR_ID1 201
class MyApp : public wxAppConsole
{
  public:
    virtual bool OnInit();
	void OnTmrEvent(wxThreadEvent& event);
};
IMPLEMENT_APP(MyApp)
bool MyApp::OnInit()
{
	ceWxTmr* t = new ceWxTmr(this,TMR_ID1,2000);// 2 seconds
	t->Start();
	Connect(TMR_ID1, wxEVT_THREAD, wxThreadEventHandler(MyApp::OnTmrEvent));
    return true;
}

void MyApp::OnTmrEvent(wxThreadEvent& event)
{
	static int c = 0;
    std::vector<char> v = event.GetPayload<std::vector<char>>();
	printf("Ev = %d\n",int(v[0]));
	if(++c >= 3){
		Exit();
	}
}