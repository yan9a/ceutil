/////////////////////////////////////////////////////////////////////////////
// Name:         ceUDP.h
// Description:  UDP communication module
// Author:       Yan Naing Aye
// Ref: http://cool-emerald.blogspot.com/2018/01/udptcp-socket-programming-with-wxwidgets.html
/////////////////////////////////////////////////////////////////////////////

#ifndef CEUDP_H
#define CEUDP_H

#include "wx/wx.h"
#include "wx/socket.h"

// this example is currently written to use only IP or only IPv6 sockets, it
// should be extended to allow using either in the future
#if wxUSE_IPV6
typedef wxIPV6address IPaddress;
#else
typedef wxIPV4address IPaddress;
#endif

#include <stdio.h>
#include <string>
#include <vector>

namespace ce {
class ceUDP : public wxEvtHandler {
public:
	ceUDP(wxAppConsole* app, int socketid, int rx_port); // rx_port = 0 for not listening
	ceUDP(wxAppConsole* app, int socketid, int rx_port, int tx_port);
	ceUDP(wxAppConsole* app, int socketid, int rx_port, int tx_port, std::string remotehost);
	int Tx(std::vector<char> bv); // return 0 = success, 1 = error
	int Tx(std::vector<char> bv, std::string remoteHost, int txPort); // return 0 = success, 1 = error
	int Open(); // return 0 = success, 1 = error
	void Close();
	int GetRxPort();
	void SetRxPort(int rx_port);
	int GetTxPort();
	void SetTxPort(int tx_port);
	std::string GetRemotehost();
	void SetRemotehost(std::string remoteHost);
private:
	void Rx(int socketid, std::vector<char> bv, std::string ip, int port);
	void OnSocketEvent(wxSocketEvent& event);
	wxAppConsole* _app;
	int SOCKET_ID;
	int udp_port_rx;
	int udp_port_tx;
	std::string udp_remote_host;
	wxDatagramSocket *sock;
	int _ok;
};

} // namespace ce 
#endif // CEUDP_H
