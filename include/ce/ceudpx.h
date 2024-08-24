/////////////////////////////////////////////////////////////////////////////
// Name:         ceUDP.h
// Description:  UDP communication module
// Author:       Yan Naing Aye
// Ref: http://cool-emerald.blogspot.com/2018/01/udptcp-socket-programming-with-wxwidgets.html
/////////////////////////////////////////////////////////////////////////////

#ifndef CEUDP_H
#define CEUDP_H

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
// this example is currently written to use only IP or only IPv4 sockets, it
// should be extended to allow using either in the future
//#if wxUSE_IPV6
//typedef wxIPV6address IPaddress;
//#else
typedef wxIPV4address IPaddress;
//#endif

#define CE_UDP_RX_BUF_SIZE 32768

namespace ce {
class ceUDP : public wxEvtHandler {
public:
	ceUDP(wxEvtHandler* app, int socketid, int rx_port); // rx_port = 0 for not listening
	ceUDP(wxEvtHandler* app, int socketid, int rx_port, int tx_port);
	ceUDP(wxEvtHandler* app, int socketid, int rx_port, int tx_port, std::string remotehost);
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
	wxEvtHandler* _app;
	int SOCKET_ID;
	int udp_port_rx;
	int udp_port_tx;
	std::string udp_remote_host;
	wxDatagramSocket *sock;
	int _ok;
};

} // namespace ce 
#endif // CE_WX
#endif // CEUDP_H
