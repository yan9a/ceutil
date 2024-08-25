/////////////////////////////////////////////////////////////////////////////
// Name:         ceudp.h
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
#include <cstdio>
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

/////////////////////////////////////////////////////////////////////////////
inline ceUDP::ceUDP(wxEvtHandler* app, int socketid, int rx_port) :
	_app(app), SOCKET_ID(socketid), udp_port_rx(rx_port), udp_port_tx(rx_port), udp_remote_host("localhost")
{
	_ok = false;
}

inline ceUDP::ceUDP(wxEvtHandler* app, int socketid, int rx_port, int tx_port) :
	_app(app), SOCKET_ID(socketid), udp_port_rx(rx_port), udp_port_tx(tx_port), udp_remote_host("localhost")
{
	_ok = false;
}

inline ceUDP::ceUDP(wxEvtHandler* app, int socketid, int rx_port, int tx_port, std::string remotehost) :
	_app(app), SOCKET_ID(socketid), udp_port_rx(rx_port), udp_port_tx(tx_port), udp_remote_host(remotehost)
{
	_ok = false;
}

inline int ceUDP::GetRxPort()
{
	return this->udp_port_rx;
}

inline void ceUDP::SetRxPort(int rx_port)
{
	this->udp_port_rx = rx_port;
}

inline int ceUDP::GetTxPort()
{
	return this->udp_port_tx;
}

inline void ceUDP::SetTxPort(int tx_port)
{
	this->udp_port_tx = tx_port;
}

inline std::string ceUDP::GetRemotehost()
{
	return this->udp_remote_host;
}

inline void ceUDP::SetRemotehost(std::string remoteHost)
{
	this->udp_remote_host = remoteHost;
}

inline int ceUDP::Open()
{
	// Create the address - defaults to localhost:0 initially
	IPaddress addr;
	addr.AnyAddress();
	if (udp_port_rx != 0) addr.Service(udp_port_rx);
	std::string tmp;
	// printf("UDP: creating socket at %s : %d \n",addr.IPAddress().ToStdString().c_str(),addr.Service());

	// Create the socket
	sock = new wxDatagramSocket(addr);

	// We use IsOk() here to see if the server is really listening
	if (!sock->IsOk()) {
		perror("ceUDP could not listen at the specified port");
		return 1;
	}

	IPaddress addrReal;
	if (!sock->GetLocal(addrReal)) {
		perror("ceUDP couldn't get the address we bound to.");
		return 1;
	}
	else {
		// std::string tmp = "UDP server listening at "+addrReal.IPAddress().ToStdString()+" : "+std::to_string(addrReal.Service());
		// printf("%s\n",tmp.c_str());
	}
	// Setup the event handler
	sock->SetEventHandler(*this, SOCKET_ID);
	sock->SetNotify(wxSOCKET_INPUT_FLAG);
	sock->Notify(true);
	Connect(SOCKET_ID, wxEVT_SOCKET, wxSocketEventHandler(ceUDP::OnSocketEvent));
	_ok = true;
	return 0;
}

inline void ceUDP::Close()
{
	_ok = false;
	sock->Close();
}

inline int ceUDP::Tx(std::vector<char> bv, std::string remoteHost, int txPort)
{
	if (!_ok) return 1;
	size_t txn = bv.size();
	// IPaddress addrLocal;
	// addrLocal.AnyAddress();
	//addrLocal.Hostname("192.168.1.10");
	// wxDatagramSocket sock2tx(addrLocal);
	// if ( !sock2tx.IsOk() )
	// {
	// 	perror("ceUDP Tx failed to create UDP socket");
	// 	return;
	// }
	// write("UDP TX: created "+ddrLocal.IPAddress().ToStdString()+" : "+to_string(addrLocal.Service()));

	IPaddress raddr;
	//raddr.Hostname("localhost");
	raddr.Hostname(remoteHost);
	raddr.Service(txPort);

	if (sock->SendTo(raddr, bv.data(), txn).LastCount() != txn)
	{
		perror("ceUDP Tx error");
		return 1;
	}
	else {
		// std::string tmp ="UDP TX: "+util::cvec2hex(bv); 
		// printf("%s\n",tmp.c_str());
		//  printf("ceUDPTx n= %ld \n",txn);
		//  fflush(stdout);
	}
	return 0;
}

inline int ceUDP::Tx(std::vector<char> bv)
{
	return this->Tx(bv, udp_remote_host, udp_port_tx);
}

inline void ceUDP::OnSocketEvent(wxSocketEvent& event)
{
	IPaddress addr;
	addr.Service(udp_port_rx);
	char buf[CE_UDP_RX_BUF_SIZE];
	size_t n;
	int socketid = event.GetId();
	std::string tmp; // = ("UDP RX: id = " + to_string(socketid));
	std::vector<char> vc;
	switch (event.GetSocketEvent())
	{
	case wxSOCKET_INPUT:
		sock->Notify(false);
		n = sock->RecvFrom(addr, buf, sizeof(buf)).LastCount();
		sock->Notify(true);
		if (!n) {
			perror("ceUDP Rx failed");
			return;
		}
		vc.assign(buf, buf + n);
		//		printf("Received \'%s\' from %s:%u. \n", 
		//			util::cvec2hex(util::cptr2cvec(buf, n)).c_str(),
		//			addr.IPAddress().ToStdString().c_str(), 
		//			addr.Service());
				// printf("ceUDPRx n= %ld \n",n);
				// fflush(stdout);
		this->Rx(socketid, vc, addr.IPAddress().ToStdString(), addr.Service());
		break;
	default:
		perror("ceUDP Rx unexpected event");
		break;
	}
}

inline void ceUDP::Rx(int socketid, std::vector<char> bv, std::string ip, int port)
{
	// ("UDP rx" + to_string(socketid) + ": " + util::cvec2hex(bv));
	wxString ws(ip);
	wxThreadEvent event(wxEVT_THREAD, socketid);
	event.SetPayload<std::vector<char>>(bv);
	event.SetString(ws);
	event.SetInt(port);
	wxQueueEvent(_app, event.Clone());
}
/////////////////////////////////////////////////////////////////////////////

} // namespace ce 
#endif // CE_WX
#endif // CEUDP_H
