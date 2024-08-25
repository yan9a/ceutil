// File: tcpsvr.cpp
// Description: A simple wxWidgets TCP server sample 
// Author: Yan Naing Aye
// Web: http://cool-emerald.blogspot.com
// MIT License (https://opensource.org/licenses/MIT)
// Copyright (c) 2018 Yan Naing Aye

// References
// [1] Guillermo Rodriguez Garcia, Vadim Zeitlin, "Server for wxSocket demo", 
//     https://github.com/wxWidgets/wxWidgets/blob/master/samples/sockets/server.cpp, 2009.
// [2] Julian Smart and Kevin Hock, "Cross-Platform GUI Programming with wxWidgets,"
//    Pearson Education, Inc. 2006. ISBN: 0-13-147381-6.

#include "ce/cetcpsvr.h"
#include "ce/cemisc.h"

// #include "myicon.xpm"
using namespace ce;

#define MAX_SOCKET_N 2
// IDs for the controls and the menu commands
enum
{
	ID_TXTRX=101,
	SERVER_ID=102,
	SOCKET_ID=200,	
	SOCKET_ID1=201,
	// menu items
	Minimal_Quit = wxID_EXIT,
	Minimal_About = wxID_ABOUT
};

class MyFrame;
class MyApp : public wxApp
{
	ceTcpServer* _tcpsvr;
	MyFrame *frame;
public:
	void OnTcpServerEvent(wxThreadEvent& event);
	void OnTcpSocketEvent(wxThreadEvent& event);
	virtual bool OnInit();
};

// Define a new frame type: this is going to be our main frame
class MyFrame : public wxFrame
{
public:
	// ctor(s)
	MyFrame(const wxString& title);
	~MyFrame();
	// event handlers (these functions should _not_ be virtual)
	void OnQuit(wxCommandEvent& event);
	void OnAbout(wxCommandEvent& event);
	void Print(std::string str);
private:
	wxTextCtrl *txtRx;
	// any class wishing to process wxWidgets events must use this macro
	wxDECLARE_EVENT_TABLE();
};

// event tables and other macros for wxWidgets
wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)
	EVT_MENU(Minimal_Quit,  MyFrame::OnQuit)
	EVT_MENU(Minimal_About, MyFrame::OnAbout)
wxEND_EVENT_TABLE()

IMPLEMENT_APP(MyApp)

// 'Main program' equivalent: the program execution "starts" here
bool MyApp::OnInit()
{
	if ( !wxApp::OnInit() )
	return false;

	// depending on your system, some ports such as 3000, 8000, 8080 should be used with cautions
	int port = 7225;
	this->_tcpsvr = new ceTcpServer(this,SERVER_ID,SOCKET_ID,port,2);
	this->_tcpsvr->Open();
	Connect(SERVER_ID, wxEVT_THREAD, wxThreadEventHandler(MyApp::OnTcpServerEvent));
	for(int i=0; i<MAX_SOCKET_N;i++){
		Connect(SOCKET_ID+i, wxEVT_THREAD, wxThreadEventHandler(MyApp::OnTcpSocketEvent));
	}
	frame = new MyFrame("TCP Server using ceUtil lib");
	frame->Show(true);
	this->frame->Print("TCP Server listening:"+std::to_string(port));
	return true;
}

void MyApp::OnTcpServerEvent(wxThreadEvent& event)
{
	std::vector<char> v = event.GetPayload<std::vector<char>>();
	std::string s = event.GetString().ToStdString();
	int i = event.GetInt();
	std::string str = "Server event: ip ="+s+", port = "+ std::to_string(i)+ (v[0]?", connected, clients: ":", disconnected, clients: ") + std::to_string(v[1])+ ", id: " + std::to_string(SOCKET_ID+int(v[2]));
	printf("%s\n",str.c_str());
	this->frame->Print(str);
}

void MyApp::OnTcpSocketEvent(wxThreadEvent& event)
{
	std::vector<char> v = event.GetPayload<std::vector<char>>();
	std::string s = event.GetString().ToStdString();
	int i = event.GetInt();
	std::string str = "Socket event: Rx vec = "+ceMisc::cvec2hex(v)+", ip ="+s+", port = "+ std::to_string(i)+ ", id: " + std::to_string(event.GetId());
	printf("%s\n",str.c_str());
	this->frame->Print(str);

	// write back
	std::vector<char> tb{'O','K'};
	this->_tcpsvr->Tx(event.GetId(),tb);
	this->frame->Print("Tx: "+ ce::ceMisc::cvec2hex(tb));
}

// frame constructor
MyFrame::MyFrame(const wxString& title)
: wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(390, 280), 
  wxDEFAULT_FRAME_STYLE ^ wxRESIZE_BORDER)
{
// // set the frame icon
// SetIcon(wxICON(sample));

#if wxUSE_MENUS
// create a menu bar
wxMenu *fileMenu = new wxMenu;

// the "About" item should be in the help menu
wxMenu *helpMenu = new wxMenu;
helpMenu->Append(Minimal_About, "&About\tF1", "Show about dialog");

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
SetStatusText("TCP server using ceUtil lib");
#endif // wxUSE_STATUSBAR
txtRx = new wxTextCtrl(this, ID_TXTRX, wxT(""), wxPoint(5, 5), 
        wxSize(365, 125), wxTE_MULTILINE);


}

MyFrame::~MyFrame()
{
	// No delayed deletion here, as the frame is dying anyway
	//delete _tcpsvr;
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
"ceUtil TCP server sample\n"
"\n"
"Author: Yan Naing Aye \n"
"Web: http://cool-emerald.blogspot.com"
),
"About ceUtil TCP server sample",
wxOK | wxICON_INFORMATION,
this);
}

void MyFrame::Print(std::string str)
{
	txtRx->AppendText(wxString::Format(wxT("%s\n"),str));
}



