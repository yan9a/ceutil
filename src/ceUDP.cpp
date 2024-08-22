/////////////////////////////////////////////////////////////////////////////
// Name:         ceUDP.cpp
// Description:  UDP communication module
// Author:       Yan Naing Aye
// Ref: http://cool-emerald.blogspot.com/2018/01/udptcp-socket-programming-with-wxwidgets.html
/////////////////////////////////////////////////////////////////////////////

#include "ce/ceUDP.h"
#if CE_WX==1
namespace ce {

ceUDP::ceUDP(wxEvtHandler* app, int socketid, int rx_port) :
_app(app),SOCKET_ID(socketid),udp_port_rx(rx_port),udp_port_tx(rx_port),udp_remote_host("localhost")
{
	_ok = false;
}

ceUDP::ceUDP(wxEvtHandler* app, int socketid, int rx_port, int tx_port) :
_app(app),SOCKET_ID(socketid),udp_port_rx(rx_port),udp_port_tx(tx_port),udp_remote_host("localhost")
{
	_ok = false;
}

ceUDP::ceUDP(wxEvtHandler* app, int socketid, int rx_port, int tx_port, std::string remotehost) :
_app(app),SOCKET_ID(socketid),udp_port_rx(rx_port),udp_port_tx(tx_port),udp_remote_host(remotehost)
{
	_ok = false;
}

int ceUDP::GetRxPort()
{
	return this->udp_port_rx;
}

void ceUDP::SetRxPort(int rx_port)
{
	this->udp_port_rx = rx_port;
}

int ceUDP::GetTxPort()
{
	return this->udp_port_tx;
}

void ceUDP::SetTxPort(int tx_port)
{
	this->udp_port_tx = tx_port;
}

std::string ceUDP::GetRemotehost()
{
	return this->udp_remote_host;
}

void ceUDP::SetRemotehost(std::string remoteHost)
{
	this->udp_remote_host = remoteHost;
}

int ceUDP::Open()
{
	// Create the address - defaults to localhost:0 initially
	IPaddress addr;
	addr.AnyAddress();
	if(udp_port_rx !=0) addr.Service(udp_port_rx);
	std::string tmp;
	// printf("UDP: creating socket at %s : %d \n",addr.IPAddress().ToStdString().c_str(),addr.Service());

	// Create the socket
	sock = new wxDatagramSocket(addr);

	// We use IsOk() here to see if the server is really listening
	if (!sock->IsOk()){
		perror("ceUDP could not listen at the specified port");
		return 1;
	}

	IPaddress addrReal;
	if (!sock->GetLocal(addrReal)){
		perror("ceUDP couldn't get the address we bound to.");       
		return 1; 
	}
	else{
		// std::string tmp = "UDP server listening at "+addrReal.IPAddress().ToStdString()+" : "+std::to_string(addrReal.Service());
		// printf("%s\n",tmp.c_str());
	}
	// Setup the event handler
	sock->SetEventHandler(*this,SOCKET_ID);
	sock->SetNotify(wxSOCKET_INPUT_FLAG);
	sock->Notify(true);
	Connect(SOCKET_ID,wxEVT_SOCKET,wxSocketEventHandler(ceUDP::OnSocketEvent));
	_ok = true;
	return 0;
}

void ceUDP::Close()
{
	_ok = false;
	sock->Close();
}

int ceUDP::Tx(std::vector<char> bv, std::string remoteHost, int txPort)
{    
	if(!_ok) return 1;
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

int ceUDP::Tx(std::vector<char> bv)
{
	return this->Tx(bv,udp_remote_host,udp_port_tx);
}

void ceUDP::OnSocketEvent(wxSocketEvent& event)
{
	IPaddress addr;
	addr.Service(udp_port_rx);
	char buf[CE_UDP_RX_BUF_SIZE];
	size_t n;
	int socketid = event.GetId();
	std::string tmp; // = ("UDP RX: id = " + to_string(socketid));
	std::vector<char> vc;
	switch(event.GetSocketEvent())
	{
	case wxSOCKET_INPUT:
		sock->Notify(false);
		n = sock->RecvFrom(addr, buf, sizeof(buf)).LastCount();
		sock->Notify(true);
		if (!n){
			perror("ceUDP Rx failed");
			return;
		}
		vc.assign(buf,buf+n);
//		printf("Received \'%s\' from %s:%u. \n", 
//			util::cvec2hex(util::cptr2cvec(buf, n)).c_str(),
//			addr.IPAddress().ToStdString().c_str(), 
//			addr.Service());
		// printf("ceUDPRx n= %ld \n",n);
		// fflush(stdout);
		this->Rx(socketid,vc,addr.IPAddress().ToStdString(),addr.Service());
		break;
	default:
		perror("ceUDP Rx unexpected event");
		break;
	}
}

void ceUDP::Rx(int socketid, std::vector<char> bv, std::string ip, int port)
{
	// ("UDP rx" + to_string(socketid) + ": " + util::cvec2hex(bv));
	wxString ws(ip);
	wxThreadEvent event(wxEVT_THREAD, socketid);
	event.SetPayload<std::vector<char>>(bv);
	event.SetString(ws);
	event.SetInt(port);
	wxQueueEvent(_app, event.Clone());
}

} // namespace ce 
#endif // CE_WX
