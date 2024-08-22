// File: tcpclient.cpp
// Description: A simple wxWidgets TCP client sample 
// Author: Yan Naing Aye
// Web: http://cool-emerald.blogspot.com
// MIT License (https://opensource.org/licenses/MIT)
// Copyright (c) 2018 Yan Naing Aye

// References
// [1] Guillermo Rodriguez Garcia, Vadim Zeitlin, "Server for wxSocket demo", 
//     https://github.com/wxWidgets/wxWidgets/blob/master/samples/sockets/server.cpp, 2009.
// [2] Julian Smart and Kevin Hock, "Cross-Platform GUI Programming with wxWidgets,"
//    Pearson Education, Inc. 2006. ISBN: 0-13-147381-6.

#include "ce/ceUtil.h"
using namespace ce;
// IDs for the controls and the menu commandscm
enum
{
	ID_BTNSEND = 101,
	ID_TXTSEND,
	ID_TXTRX,
	SOCKET_ID,
	CLIENT_OPEN = wxID_OPEN,
	CLIENT_CLOSE = wxID_CLOSE,
	// menu items
	Minimal_Quit = wxID_EXIT,
	Minimal_About = wxID_ABOUT
};

class MyFrame;
class MyApp : public wxApp
{
	MyFrame *frame;
public:
	ceTcpClient* _tcpclient;
	void OnTcpSocketEvent(wxThreadEvent& event);
	virtual bool OnInit();
};

// Define a new frame type: this is going to be our main frame
class MyFrame : public wxFrame
{
public:
	// ctor(s)
	MyFrame(MyApp* app, const wxString& title);
	~MyFrame();
	// event handlers (these functions should _not_ be virtual)
	void OnQuit(wxCommandEvent& event);
	void OnAbout(wxCommandEvent& event);
	void Print(std::string str);
	void OnOpenConnection(wxCommandEvent& event);
	void OnCloseConnection(wxCommandEvent& event);
	void OnSend(wxCommandEvent& event);
	void UpdateStatusBar();
private:
	MyApp* _app;
	wxButton* btnSend;
	wxTextCtrl* txtSend;
	wxTextCtrl *txtRx;
	wxMenu* fileMenu;
	wxMenu* helpMenu;
	// any class wishing to process wxWidgets events must use this macro
	wxDECLARE_EVENT_TABLE();
};

// event tables and other macros for wxWidgets
wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)
	EVT_MENU(Minimal_Quit, MyFrame::OnQuit)
	EVT_MENU(Minimal_About, MyFrame::OnAbout)
	EVT_MENU(CLIENT_OPEN, MyFrame::OnOpenConnection)
	EVT_MENU(CLIENT_CLOSE, MyFrame::OnCloseConnection)
wxEND_EVENT_TABLE()

IMPLEMENT_APP(MyApp)

// 'Main program' equivalent: the program execution "starts" here
bool MyApp::OnInit()
{
	if ( !wxApp::OnInit() )
	return false;

	// depending on your system, some ports such as 3000, 8000, 8080 should be used with cautions
	this->_tcpclient = new ceTcpClient(this,SOCKET_ID);	
	this->_tcpclient->SetRemote("localhost",7225);
	Connect(SOCKET_ID, wxEVT_THREAD, wxThreadEventHandler(MyApp::OnTcpSocketEvent));
	frame = new MyFrame(this,"TCP Client using ceUtil lib");
	frame->Show(true);
	this->frame->Print("Click Open Session menu to start a connection");
	return true;
}

void MyApp::OnTcpSocketEvent(wxThreadEvent& event)
{
	std::vector<char> v = event.GetPayload<std::vector<char>>();
	std::string s = event.GetString().ToStdString();
	int i = event.GetInt();
	std::string str = "Socket event: Rx vec = "+ceMisc::cvec2hex(v)+", ip ="+s+", port = "+ std::to_string(i);
	printf("%s\n",str.c_str());
	this->frame->Print(str);
}

// frame constructor
MyFrame::MyFrame(MyApp* app, const wxString& title)
: wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(390, 280), 
  wxDEFAULT_FRAME_STYLE ^ wxRESIZE_BORDER),_app(app)
{
#if wxUSE_MENUS
// create a menu bar
wxMenu *fileMenu = new wxMenu;

// the "About" item should be in the help menu
wxMenu *helpMenu = new wxMenu;
helpMenu->Append(Minimal_About, "&About\tF1", "Show about dialog");

fileMenu->Append(CLIENT_OPEN, "&Open session\tAlt-O", "Connect to server");
fileMenu->Append(CLIENT_CLOSE, "&Close session\tAlt-C", "Close connection");
fileMenu->Append(Minimal_Quit, "E&xit\tAlt-X", "Quit this program");

// now append the freshly created menu to the menu bar...
wxMenuBar *menuBar = new wxMenuBar();
menuBar->Append(fileMenu, "&File");
menuBar->Append(helpMenu, "&Help");

// ... and attach this menu bar to the frame
SetMenuBar(menuBar);
#endif // wxUSE_MENUS

#if wxUSE_STATUSBAR
// create a status bar just for fun (by default with 1 pane only)
CreateStatusBar(2);
SetStatusText("TCP client using ceUtil lib");
#endif // wxUSE_STATUSBAR
btnSend = new wxButton(this, ID_BTNSEND, wxT("Send"),
	wxPoint(5, 5), wxSize(100, 25));
txtSend = new wxTextCtrl(this, ID_TXTSEND, wxT("Hello!"),
	wxPoint(120, 5), wxSize(250, 25));
txtRx = new wxTextCtrl(this, ID_TXTRX, wxT(""),
	wxPoint(5, 35), wxSize(365, 125), wxTE_MULTILINE);

Connect(ID_BTNSEND, wxEVT_COMMAND_BUTTON_CLICKED,
	wxCommandEventHandler(MyFrame::OnSend));
}

MyFrame::~MyFrame()
{
	// No delayed deletion here, as the frame is dying anyway
}

// event handlers
void MyFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
// true is to force the frame to close
Close(true);
}

void MyFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
wxMessageBox(wxString::Format
(
"ceUtil TCP client sample\n"
"\n"
"Author: Yan Naing Aye \n"
"Web: http://cool-emerald.blogspot.com"
),
"About ceUtil TCP client sample",
wxOK | wxICON_INFORMATION,
this);
}

void MyFrame::Print(std::string str)
{
	txtRx->AppendText(wxString::Format(wxT("%s\n"),str));
}

void MyFrame::UpdateStatusBar()
{
	//fileMenu->Enable(CLIENT_OPEN, !_app->_tcpclient->IsConnected());
	//fileMenu->Enable(CLIENT_CLOSE, _app->_tcpclient->IsConnected());
	//if (_app->_tcpclient->IsConnected()) {
	//	//SetStatusText(wxString::Format(wxT("%s:%u"), 
	//	//    addr.IPAddress(), addr.Service()), 1);
	//	SetStatusText(wxString::Format(wxT("Connected")), 1);
	//}
	//else {
	//	SetStatusText(wxString::Format(wxT("Not connected")), 1);
	//}
}

void MyFrame::OnOpenConnection(wxCommandEvent& WXUNUSED(event))
{
	_app->_tcpclient->Open();
	// fileMenu->Enable(CLIENT_OPEN, false);
    // fileMenu->Enable(CLIENT_CLOSE, false);
	//update status
	this->UpdateStatusBar();
}

void MyFrame::OnCloseConnection(wxCommandEvent& WXUNUSED(event))
{
	_app->_tcpclient->Close();

	//update status
	this->UpdateStatusBar();
}

void MyFrame::OnSend(wxCommandEvent& WXUNUSED(event))
{
	wxString str = txtSend->GetValue();
	wxCharBuffer buffer = str.ToUTF8();
	size_t txn = buffer.length();//for non-ASCII chars, having more than one byte per char
	std::vector<char> vc{buffer.data(), buffer.data()+txn};
	_app->_tcpclient->Tx(vc);
}