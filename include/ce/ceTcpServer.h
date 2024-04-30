/////////////////////////////////////////////////////////////////////////////
// Name:         ceTcpServer.h
// Description:  TCP server module
// Author:       Yan Naing Aye
// Ref: http://cool-emerald.blogspot.com/2018/01/udptcp-socket-programming-with-wxwidgets.html
/////////////////////////////////////////////////////////////////////////////

#ifndef CETCPSVR_H
#define CETCPSVR_H
#include "ce/ceMacros.h" // macros
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
#define CE_TCPSVR_RX_BUF_SIZE 32768

namespace ce {
class ceTcpServer : public wxEvtHandler {
public:
	ceTcpServer(wxAppConsole* app, int serverid, int socketid, int port); // port = listening port number
    ~ceTcpServer();
	
	bool Open(); // return true = success, false = error
	bool Close();

    // Transmit (write) 
    int Tx(std::vector<char> bv); // return 0 = success, 1 = error
    // Receive (read) will raise receive event
private:
	void OnServerEvent(wxSocketEvent& event);
    void OnSocketEvent(wxSocketEvent& event);
	wxAppConsole* _app;
	int _socket_id;
    int _server_id;
	int _port;
	bool _listening{false};
    std::string _remote_host{""};
    int _remote_port{0};
    wxSocketServer* _socketServer{};
    wxSocketBase* _socketBase{};
	void PrintLog(std::string str);
	int _connected_n{0};
};

} // namespace ce 
#endif // CE_WX
#endif // CETCPSVR_H
