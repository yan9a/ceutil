/////////////////////////////////////////////////////////////////////////////
// Name:         ceTcpClient.h
// Description:  TCP client module
// Author:       Yan Naing Aye
// Ref: http://cool-emerald.blogspot.com/2018/01/udptcp-socket-programming-with-wxwidgets.html
/////////////////////////////////////////////////////////////////////////////

#ifndef CETCPCLIENT_H
#define CETCPCLIENT_H

#ifndef CE_MACROS_H
#define CE_MACROS_H

#define CE_WX 1 // include WX
#define CE_DBG_PRINT 0 // print dbg mes

#if defined(_WIN64) || defined(__WIN32__) || defined(_WIN32) || defined(WIN32) || defined(__WINDOWS__) || defined(__TOS_WIN__) || defined(__CYGWIN__)
	#ifndef CE_WINDOWS
		#define CE_WINDOWS
	#endif
#elif defined(__linux__) || defined(unix) || defined(__unix) || defined(__unix__)
	#ifndef CE_LINUX
		#define CE_LINUX
	#endif
#else
	#ifndef CE_NOS
		#define CE_NOS
	#endif
#endif

#if defined(__x86_64) || defined(__x86_64__) || defined(__amd64__) || defined(__amd64)
	#ifndef CE_x86_64
		#define CE_x86_64
	#endif
#elif defined(__arm__) || defined(_M_ARM)
	#ifndef CE_ARM
		#define CE_ARM
	#endif
#else
	#ifndef CE_NARCH
		#define CE_NARCH
	#endif
#endif

#endif // CE_MACROS_H

#if CE_WX==1
#include <stdio.h>
#include <string>
#include <vector>
#include "wx/wx.h"
#include "wx/socket.h"
#include "ce/ceMisc.h" // utilities
// this example is currently written to use only IP or only IPv4 sockets, it
// should be extended to allow using either in the future
//#if wxUSE_IPV6
//typedef wxIPV6address IPaddress;
//#else
typedef wxIPV4address IPaddress;
//#endif
#define CE_TCPCLIENT_RX_BUF_SIZE 32768

namespace ce {
class ceTcpClient : public wxEvtHandler {
public:
	ceTcpClient(wxEvtHandler* app, int socketid); // port = listening port number
    ~ceTcpClient();
	
	void Open(); 
	void Close();

    // Transmit (write) 
    int Tx(std::vector<char> bv); // return 0 = success, 1 = error
    // Receive (read) will raise receive event

	void SetRemote(std::string remotehost, int port);
	bool IsConnected();
	bool IsOK();
	void SetRemotehost(std::string remotehost);
	void SetPort(int port);
	std::string GetRemotehost();
	int GetPort();
private:
    void OnSocketEvent(wxSocketEvent& event);
	wxEvtHandler* _app;
	int _socket_id;
	int _port{ 0 };
	std::string _remotehost{""};
    wxSocketClient* _socketClient{};
	void PrintLog(std::string str);
};

} // namespace ce 
#endif // CE_WX
#endif // CETCPCLIENT_H
