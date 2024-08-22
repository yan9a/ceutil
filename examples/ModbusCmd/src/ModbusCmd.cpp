// File: ModbusCmd.cpp
// Description: Modbus RTS communication for RS485 switches
// WebSite: http://cool-emerald.blogspot.sg/2017/05/serial-port-programming-in-c-with.html
// MIT License (https://opensource.org/licenses/MIT)
// Copyright (c) 2021 Yan Naing Aye

#include "wx/wx.h"
#include <wx/spinctrl.h>
#include <string>
#include <vector>
#include <cstdint>
#include <wx/numdlg.h>
// ----------------------------------------------------------------------------
// private classes
#include "ce/ceUtil.h"
using namespace std;
using namespace ce;
// ----------------------------------------------------------------------------
#define MODBUS_ID_MAX 31
#define MODBUS_ADDR_MAX 16
#define MODBUS_ACTION_MAX 8
#define MODBUS_DURATION_MAX 255
#define N_LOG_LIST_LINES 30

#define MODBUS_DIALOG_BTN_SEND 100
#define MODBUS_DIALOG_BTN_CMD 101

#define MODBUS_DIALOG_SPN_ID 102
#define MODBUS_DIALOG_SPN_ADDR 103
#define MODBUS_DIALOG_CBO_FUNC 104
#define MODBUS_DIALOG_CBO_ACTION 105
#define MODBUS_DIALOG_SPN_DURATION 106
#define MODBUS_DIALOG_TXT_CMD 107
#define ID_BAUDRATE_MNU 108
#define ID_TIMER 109
#define ID_WXSERIAL 110
#define ID_SELPORT_MNU 111
#define ID_OPEN_MNU 112
#define ID_CLOSE_MNU 113
#define ID_CLEAR_MNU 114
#define GUI_LST_LOG_ID 115
// wxID_EXIT for quit
// wxID_ABOUT for about
// ----------------------------------------------------------------------------
class MyFrame;
class MyApp : public wxApp
{
public:
    virtual bool OnInit();
	MyFrame* _fra;
	ceWxSerial* _com;
	ceModbus* _modbus;
	void OnSerialEvent(wxThreadEvent& event);
};

// Define a new frame type: this is going to be our main frame
class MyFrame : public wxFrame
{
public:
    // ctor(s)
    MyFrame(MyApp* app, const wxString& title);

    // event handlers (these functions should _not_ be virtual)
    void OnQuit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
	void OnOpen(wxCommandEvent& event);
	void OnClose(wxCommandEvent& event);
	void SelPort(wxCommandEvent& event);
	void SetBaud(wxCommandEvent& event);
	void OnClear(wxCommandEvent& event);
	void OnSend(wxCommandEvent& event);
	void OnTimer(wxTimerEvent& event);
	void OnSendHex(wxCommandEvent& event);
	void WriteList(std::string mes);
	void ModbusControl(uint8_t id, uint8_t func, uint16_t addr, uint8_t action, uint8_t delay);
	void ModbusCmd(std::vector<uint8_t> cmd);
private:
	MyApp* _app;
	wxSpinCtrl* spnId;
	wxSpinCtrl* spnAddr;
	wxSpinCtrl* spnDuration;
	wxButton* btnSend;
	wxComboBox* cboFunc;
	wxComboBox* cboAction;
	wxTextCtrl* txtCmd;
	wxButton* btnCmd;
	wxTimer tmr;
	wxListBox* logList;
};

IMPLEMENT_APP(MyApp)

bool MyApp::OnInit()
{
    if ( !wxApp::OnInit() )
        return false;
    _fra = new MyFrame(this, wxT("Modbus RS485 Relay Board Control"));

	_modbus = new ceModbus();
	string device;
#if defined(CE_WINDOWS)
	device = "\\\\.\\COM1";
#else
	device = "/dev/ttyUSB0";
#endif
	_com = new ceWxSerial(this, ID_WXSERIAL, 10, device, 9600, 8, 'N', 1);
	Connect(ID_WXSERIAL, wxEVT_THREAD, wxThreadEventHandler(MyApp::OnSerialEvent));
    _fra->Show(true);
    return true;
}

void MyApp::OnSerialEvent(wxThreadEvent& event)
{
	std::vector<char> v = event.GetPayload<std::vector<char>>();
	char ch;
	for (int i = 0; i < v.size(); i++) {
		ch = v[i];
		// printf("%02X ", (unsigned int)ch & 0xFF);
		if (_modbus->ReceiveRxFrame(ch)) {
			char* buf = _modbus->GetRxBuf();
			size_t rn = _modbus->GetRxN();
			if (buf[1] == 6) {
				_fra->WriteList("Received control reply");
			}
			else if (buf[1] == 3) {
				_fra->WriteList("Received read reply");
				std::vector<uint16_t> rstatus = _modbus->GetStatus(buf, rn);
				for (uint16_t& s : rstatus) {
					_fra->WriteList("Status: "+ to_string(s));
				}
			}
		}
	}
	// wxMessageBox(wxT("Serial event"));
}

// ----------------------------------------------------------------------------
// main frame
// ----------------------------------------------------------------------------

// frame constructor
MyFrame::MyFrame(MyApp* app, const wxString& title)
       : wxFrame(NULL, wxID_ANY, title,wxDefaultPosition,wxSize(480, 600), wxDEFAULT_FRAME_STYLE ^ wxRESIZE_BORDER),tmr(this, ID_TIMER), _app(app)
{

#if wxUSE_MENUS
    // create a menu bar
    wxMenu *fileMenu = new wxMenu;

	//Edit menu
	wxMenu *editMenu = new wxMenu;

    // the "About" item should be in the help menu
    wxMenu *helpMenu = new wxMenu;

    helpMenu->Append(wxID_ABOUT, wxT("&About\tF1"), wxT("Show about dialog"));
	Connect(wxID_ABOUT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MyFrame::OnAbout));

    fileMenu->Append(ID_OPEN_MNU, wxT("&Open\tAlt-O"), wxT("Open serial port"));
	Connect(ID_OPEN_MNU, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MyFrame::OnOpen));
	fileMenu->Append(ID_CLOSE_MNU, wxT("&Close\tAlt-C"), wxT("Close serial port"));
	Connect(ID_CLOSE_MNU, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MyFrame::OnClose));
	editMenu->Append(ID_CLEAR_MNU, wxT("Clea&r\tAlt-R"), wxT("Clear"));
	Connect(ID_CLEAR_MNU, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MyFrame::OnClear));
	fileMenu->Append(ID_SELPORT_MNU, wxT("&Serial Port\tAlt-S"), wxT("Select serial port"));
	Connect(ID_SELPORT_MNU, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MyFrame::SelPort));
	fileMenu->Append(ID_BAUDRATE_MNU, wxT("&Baud Rate\tAlt-B"), wxT("Set baud rate"));
	Connect(ID_BAUDRATE_MNU, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MyFrame::SetBaud));
	fileMenu->Append(wxID_EXIT, wxT("E&xit\tAlt-X"), wxT("Quit this program"));
	Connect(wxID_EXIT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MyFrame::OnQuit));

    // now append the freshly created menu to the menu bar...
    wxMenuBar *menuBar = new wxMenuBar();
    menuBar->Append(fileMenu, wxT("&File"));
	menuBar->Append(editMenu, wxT("&Edit"));
    menuBar->Append(helpMenu, wxT("&Help"));

    // ... and attach this menu bar to the frame
    SetMenuBar(menuBar);
#endif // wxUSE_MENUS

#if wxUSE_STATUSBAR
    // create a status bar just for fun (by default with 1 pane only)
    CreateStatusBar(2);
    SetStatusText(wxT("Modbus switch control"));
#endif // wxUSE_STATUSBAR

	// id 0 - 31
	wxStaticText* lblId = new wxStaticText(this, -1, wxT("Board:"),
		wxPoint(10, 25), wxSize(55, 30));
	this->spnId = new wxSpinCtrl(this, MODBUS_DIALOG_SPN_ID, wxT("1"),
		wxPoint(80, 20), wxSize(120, 25), wxTE_PROCESS_ENTER, 0, MODBUS_ID_MAX, 0, wxT("ID"));

	// function
	wxStaticText* lblFunc = new wxStaticText(this, -1, wxT("Func:"),
		wxPoint(230, 25), wxSize(55, 30));
	wxArrayString funcList;
	funcList.Add("6 - Control");
	funcList.Add("3 - Read");
	cboFunc = new wxComboBox(this, MODBUS_DIALOG_CBO_FUNC, wxT("6 - Control"),
		wxPoint(290, 20), wxSize(120, 25), funcList, wxCB_READONLY);

	// Address
	wxStaticText* lblAddr = new wxStaticText(this, -1, wxT("Relay:"),
		wxPoint(10, 85), wxSize(60, 30));
	this->spnAddr = new wxSpinCtrl(this, MODBUS_DIALOG_SPN_ADDR, wxT("1"),
		wxPoint(80, 80), wxSize(120, 25), wxTE_PROCESS_ENTER, 0, MODBUS_ADDR_MAX, 0, wxT("RELAY"));

	// delay
	wxStaticText* lblDuration = new wxStaticText(this, -1, wxT("Seconds:"),
		wxPoint(10, 145), wxSize(60, 30));
	this->spnDuration = new wxSpinCtrl(this, MODBUS_DIALOG_SPN_DURATION, wxT("0"),
		wxPoint(80, 140), wxSize(120, 25), wxTE_PROCESS_ENTER, 0, MODBUS_DURATION_MAX, 500);

	// Action
	wxStaticText* lblAction = new wxStaticText(this, -1, wxT("Action:"),
		wxPoint(230, 85), wxSize(55, 30));
	wxArrayString actList;
	actList.Add("0 - Default");
	actList.Add("1 - Open");
	actList.Add("2 - Close");
	actList.Add("3 - Toggle");
	actList.Add("4 - Latch");
	actList.Add("5 - Momentary");
	actList.Add("6 - Delay");
	actList.Add("7 - Open all");
	actList.Add("8 - Close all");
	cboAction = new wxComboBox(this, MODBUS_DIALOG_CBO_ACTION, wxT("3 - Toggle"),
		wxPoint(290, 80), wxSize(120, 30), actList, wxCB_READONLY);

	this->btnSend = new wxButton(this, MODBUS_DIALOG_BTN_SEND, wxT("Send"),
		wxPoint(310, 140), wxSize(100, 30));
	Connect(MODBUS_DIALOG_BTN_SEND, wxEVT_COMMAND_BUTTON_CLICKED,
		wxCommandEventHandler(MyFrame::OnSend));

	wxStaticText* lblDes = new wxStaticText(this, -1, wxT("For the following hex cmd, CRC will be appended"),
		wxPoint(10, 180), wxSize(-1,-1));
	wxStaticText* lblCmd = new wxStaticText(this, -1, wxT("Cmd:"),
		wxPoint(10, 215), wxSize(55, 30));
	this->txtCmd = new wxTextCtrl(this, MODBUS_DIALOG_TXT_CMD, wxT("01 06 00 0A 03 00"),
		wxPoint(80, 210), wxSize(210, 30));
	this->btnCmd = new wxButton(this, MODBUS_DIALOG_BTN_CMD, wxT("Send Cmd"),
		wxPoint(310, 210), wxSize(100, 30));
	Connect(MODBUS_DIALOG_BTN_CMD, wxEVT_COMMAND_BUTTON_CLICKED,
		wxCommandEventHandler(MyFrame::OnSendHex));

	wxArrayString astr;
	logList = new wxListBox(this, GUI_LST_LOG_ID, wxPoint(10,270), wxSize(400, 230), astr, wxLB_HSCROLL);

	Connect(ID_TIMER, wxEVT_TIMER, wxTimerEventHandler(MyFrame::OnTimer));
	tmr.Start(250);
}

void MyFrame::WriteList(string mes)
{
	int nl = logList->GetCount();
	if (nl >= N_LOG_LIST_LINES) {
		logList->Delete(0);
	}
	wxString wstr = mes;
	//wxDateTime wdt;
	//wdt.SetToCurrent();
	//wstr = wdt.Format(wxT("%Y-%m-%d %H:%M:%S"), wxDateTime::Local)+" : "+wstr; // add timestamp
	wxArrayString astr;
	astr.Add(wstr);
	nl = logList->GetCount();
	logList->InsertItems(astr, nl);
	logList->SetSelection(nl);
}

// event handlers

void MyFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    // true is to force the frame to close
    Close(true);
}

void MyFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
    wxMessageBox(wxString::Format(
                                  wxT("Modbus Switch Command Utility \n ")
                                  wxT("Author: Yan Naing Aye \n ")
                                  wxT("Web: https://github.com/yan9a/serial")
                                  ),
        wxT("About Modbus Switch"),
        wxOK | wxICON_INFORMATION,
        this);
}

void MyFrame::OnOpen(wxCommandEvent& WXUNUSED(event))
{
	if(_app->_com->Open()) this->WriteList(wxString::Format(wxT("Error opening port %s.\n"),_app->_com->GetPort()).ToStdString());
	else this->WriteList(wxString::Format(wxT("Port %s is opened.\n"), _app->_com->GetPort()).ToStdString());
}

void MyFrame::OnClose(wxCommandEvent& WXUNUSED(event))
{
	_app->_com->Close();
	this->WriteList(wxString::Format(wxT("Port %s is closed.\n"), _app->_com->GetPort()).ToStdString());
}

void MyFrame::SelPort(wxCommandEvent& WXUNUSED(event))
{
	if (_app->_com->IsOpened()) {
		this->WriteList(wxString::Format(wxT("Close Port %s first.\n"), _app->_com->GetPort()).ToStdString());
	}
	else {
        wxString cdev=wxString::Format(wxT("%s"), _app->_com->GetPort());
		wxString device = wxGetTextFromUser(wxT("Enter the port"), wxT("Set Port"), cdev);
		string str = device.ToStdString();
		if (str.length() > 0) {
			_app->_com->SetPortName(str);
		}

        this->WriteList(wxString::Format(wxT("Port: %s\n"), _app->_com->GetPort()).ToStdString());
	}
}

void MyFrame::SetBaud(wxCommandEvent& WXUNUSED(event))
{
	if (_app->_com->IsOpened()) {
		this->WriteList(wxString::Format(wxT("Close port %s first.\n"), _app->_com->GetPort()).ToStdString());
	}
	else {
		long n = wxGetNumberFromUser(wxT("Enter the baud rate"), wxT("Baud rate"), wxT("Set Baud Rate"),_app->_com->GetBaudRate(),0, 1000000);
		if (n >= 0) {
			_app->_com->SetBaudRate(n);
		}
		this->WriteList(wxString::Format(wxT("Baud rate: %ld\n"), _app->_com->GetBaudRate()).ToStdString());
	}
}

void MyFrame::OnSend(wxCommandEvent& WXUNUSED(event))
{
	uint8_t board = uint8_t(this->spnId->GetValue());

	uint8_t func;
	int v = this->cboFunc->GetSelection();
	if (v == wxNOT_FOUND || v == 0) func = 6;
	else func = 3;

	uint16_t addr = uint16_t(this->spnAddr->GetValue());

	v = this->cboAction->GetSelection();
	if (v == wxNOT_FOUND || func == 3) v = 0;
	uint8_t action = uint8_t(v);

	uint8_t delay = uint8_t(this->spnDuration->GetValue());
	if (func == 3) delay = 1; // to read 1 relay when not using custom cmd
	else if (action != 6) delay = 0;
	this->ModbusControl(board, func, addr, action, delay);
}

void MyFrame::OnSendHex(wxCommandEvent& WXUNUSED(event))
{
	wxString ws = this->txtCmd->GetValue();
	std::string str = ws.ToStdString();
	vector<char> v = ceMisc::hex2cvec(str);
	vector<uint8_t> cmd(v.begin(), v.end());
	this->ModbusCmd(cmd);
}

void MyFrame::ModbusControl(uint8_t id, uint8_t func, uint16_t addr, uint8_t action, uint8_t delay)
{
	string str = "Board = " + to_string(id) + ", func = " + to_string(func)
		+ ", switch = " + to_string(addr);
	if (func == 6) {
		str += ", action = " + to_string(action) + ", delay = " + to_string(delay);
	}
	
	uint8_t ah = uint8_t((addr >> 8) & 0xFF);
	uint8_t al = uint8_t(addr & 0xFF);
	std::vector<uint8_t> cmd = { id,func,ah,al,action,delay };
	this->_app->_modbus->SetCmd(cmd);
	if (this->_app->_com->Write(this->_app->_modbus->GetTxBuf(), (long)this->_app->_modbus->GetTxN())) {
		this->WriteList(str);
	}
	else {
		this->WriteList("Error in sending command");
	}
}

void MyFrame::ModbusCmd(vector<uint8_t> cmd)
{
	vector<char> v(cmd.begin(), cmd.end());
	string str = "Cmd = " + ceMisc::cvec2hex(v);
	this->_app->_modbus->SetCmd(cmd);
	if (this->_app->_com->Write(this->_app->_modbus->GetTxBuf(), (long)this->_app->_modbus->GetTxN())) {
		this->WriteList(str);
	}
	else {
		this->WriteList("Error in sending command");
	}

}

void MyFrame::OnTimer(wxTimerEvent& WXUNUSED(event))
{
	this->_app->_modbus->Tick();
}

void MyFrame::OnClear(wxCommandEvent& WXUNUSED(event))
{
	logList->Clear();
}
