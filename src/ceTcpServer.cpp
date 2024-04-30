/////////////////////////////////////////////////////////////////////////////
// Name:         ceTcpServer.cpp
// Description:  TCP server module
// Author:       Yan Naing Aye
// Ref: http://cool-emerald.blogspot.com/2018/01/udptcp-socket-programming-with-wxwidgets.html
/////////////////////////////////////////////////////////////////////////////
#include <cstdio>
#include "ce/ceTcpServer.h"
using namespace std;

#if CE_WX==1
namespace ce {

void ceTcpServer::PrintLog(string str)
{
    // printf("%s \n",str.c_str());
}

ceTcpServer::ceTcpServer(wxAppConsole* app, int serverid, int socketid, int port):
_app(app),_server_id(serverid),_socket_id(socketid),_port(port)
{    
    Connect(serverid, wxEVT_SOCKET, wxSocketEventHandler(ceTcpServer::OnServerEvent));
	Connect(socketid, wxEVT_SOCKET, wxSocketEventHandler(ceTcpServer::OnSocketEvent));
}

ceTcpServer::~ceTcpServer()
{
    this->Close();
    Disconnect(_socket_id, wxEVT_SOCKET);
	Disconnect(_server_id, wxEVT_SOCKET);
}

bool ceTcpServer::Open()
{
    if(this->_listening) {return true;} // already listening

    // Create the address - defaults to localhost:0 initially
    IPaddress addr;
    addr.AnyAddress();
    addr.Service(this->_port);
    PrintLog("Creating server at " +addr.IPAddress().ToStdString()+":"+ to_string(addr.Service()));

    // Create the socket
    this->_socketServer = new wxSocketServer(addr);

    // We use IsOk() here to see if the server is really listening
    if (!this->_socketServer->IsOk()){
        perror("ceTcpServer could not listen at the specified port");
        return false;
    }

    IPaddress addrReal;
    if (!this->_socketServer->GetLocal(addrReal)){
        perror("ceTcpServer couldn't get the address it bounds to");
    }
    else{
        PrintLog("Server listening at "+addrReal.IPAddress().ToStdString()+":"+to_string(addrReal.Service()));
    }

    // Setup the event handler and subscribe to connection events
    this->_socketServer->SetEventHandler(*this, _server_id);
    this->_socketServer->SetNotify(wxSOCKET_CONNECTION_FLAG);
    this->_socketServer->Notify(true);
    this->_listening = true;
    this->_connected_n = 0;
    return true;
}

bool ceTcpServer::Close()
{
    if(!this->_listening) {return true;} // already closed
    if(_socketBase != nullptr){
        _socketBase->Destroy();
        _socketBase = nullptr;
    }

    if(_socketServer->IsClosed()) {
        return true;
    }
    _socketServer->Discard();
    return _socketServer->Destroy();
}

void ceTcpServer::OnServerEvent(wxSocketEvent& event)
{
	PrintLog("OnServerEvent");
	wxSocketBase *sockBase;
    IPaddress addr;

	switch (event.GetSocketEvent())
	{
	case wxSOCKET_CONNECTION: 
        PrintLog("OnServerEvent"); 
        break;
	default: 
        perror("Unexpected event"); 
        return; // return if unknown
        break;
	}

	// Accept new connection if there is one in the pending
	// connections queue, else exit. We use Accept(false) for
	// non-blocking accept (although if we got here, there
	// should ALWAYS be a pending connection).

	sockBase = _socketServer->Accept(false);

	if (sockBase)
	{		
		if (!sockBase->GetPeer(addr))
		{
			PrintLog("New connection from unknown client accepted");
		}
		else
		{
			PrintLog("New client connection from "+addr.IPAddress().ToStdString()+":"+to_string(addr.Service()));
		}
	}
	else
	{
		PrintLog("Couldn't accept a new connection");
		return;
	}

	sockBase->SetEventHandler(*this, _socket_id);
	sockBase->SetNotify(wxSOCKET_INPUT_FLAG | wxSOCKET_LOST_FLAG);
	sockBase->Notify(true);

    _socketBase = sockBase; // point to last connected socket
	this->_connected_n++;
	PrintLog(to_string(this->_connected_n) + " clients connected");

    string ip = addr.IPAddress().ToStdString();
    int port = addr.Service();
    
    // 1st char: flag 1=new con, 0 = lost con, 
    // 2nd char: number of con
    wxString ws(ip);
    wxThreadEvent eventsvr(wxEVT_THREAD, _server_id);
    std::vector<char> bv{1,(char)this->_connected_n};
    eventsvr.SetPayload<std::vector<char>>(bv);
    eventsvr.SetString(ws);
    eventsvr.SetInt(port);
    wxQueueEvent(_app, eventsvr.Clone());
}

void ceTcpServer::OnSocketEvent(wxSocketEvent& event)
{
	PrintLog("OnSocketEvent");
	wxSocketBase *sockBase = event.GetSocket();
    IPaddress addr;    
    std::vector<char> vc;
    char buf[CE_TCPSVR_RX_BUF_SIZE];

    if (!sockBase->GetPeer(addr))
    {
        PrintLog("Socket event from unknown client");
    }
    else
    {
        PrintLog("Socket event from "+addr.IPAddress().ToStdString()+":" + to_string(addr.Service()));
    }
    string ip = addr.IPAddress().ToStdString();
    int port = addr.Service();
    wxString ws(ip);

	switch(event.GetSocketEvent())
	{
	case wxSOCKET_INPUT: 
        {
        PrintLog("wxSOCKET_INPUT"); 
        // We disable input events, so that the test doesn't trigger
		// wxSocketEvent again.
		sockBase->SetNotify(wxSOCKET_LOST_FLAG);

		// Receive data from socket and send it back. We will first
		// get a byte with the buffer size, so we can specify the
		// exact size and use the wxSOCKET_WAITALL flag. Also, we
		// disabled input events so we won't have unwanted reentrance.
		// This way we can avoid the infamous wxSOCKET_BLOCK flag.
        // sockBase->SetFlags(wxSOCKET_WAITALL);	
		
		// Read the message
		size_t lenRd = sockBase->Read(buf, sizeof(buf)).LastCount();
		if (!lenRd)		{
			PrintLog("Failed to read message");
			return;
		}
		else {
			PrintLog("Read "+to_string(lenRd)+" bytes");
		}
		// Enable input events again.
		sockBase->SetNotify(wxSOCKET_LOST_FLAG | wxSOCKET_INPUT_FLAG);

        // string str(buf,lenRd);
        // PrintLog("Rx: " + str);
        
        vc.assign(buf,buf+lenRd);
        wxThreadEvent eventskt(wxEVT_THREAD, _socket_id);
        eventskt.SetPayload<std::vector<char>>(vc);
        eventskt.SetString(ws);
        eventskt.SetInt(port);
        wxQueueEvent(_app, eventskt.Clone());
        }
        break;
	case wxSOCKET_LOST: 
        {
        PrintLog("wxSOCKET_LOST"); 
        if(sockBase==_socketBase){
            PrintLog("Currently pointed socket lost");
            _socketBase = nullptr;
        }
        else {
            PrintLog("Unpointed socket lost");
        }
       
        std::vector<char> bv{0,(char)this->_connected_n};
        // 1st char: flag 1=new con, 0 = lost con, 
        // 2nd char: number of con
        
        wxThreadEvent eventsvr(wxEVT_THREAD, _server_id);
        eventsvr.SetPayload<std::vector<char>>(bv);
        eventsvr.SetString(ws);
        eventsvr.SetInt(port);
        wxQueueEvent(_app, eventsvr.Clone());
		// Destroy() should be used instead of delete wherever possible,
		// due to the fact that wxSocket uses 'delayed events' (see the
		// documentation for wxPostEvent) and we don't want an event to
		// arrive to the event handler (the frame, here) after the socket
		// has been deleted. Also, we might be doing some other thing with
		// the socket at the same time; for example, we might be in the
		// middle of a test or something. Destroy() takes care of all
		// this for us.

		PrintLog("Deleting socket");
		sockBase->Destroy();
        this->_connected_n--;
	    PrintLog(to_string(this->_connected_n) + " clients connected");
        }
        break;
	default: 
        perror("Unexpected event"); 
        break;
	}
}

int ceTcpServer::Tx(std::vector<char> bv)
{
    size_t txn = 0;
    if(this->_socketBase!=nullptr){
        if(this->_socketBase->IsOk() && this->_socketBase->IsConnected()) {
                // Write to pointed client
                txn = bv.size();
                _socketBase->Write(bv.data(), txn);
                PrintLog("Tx: " + ceMisc::cvec2hex(bv));
        }
    }
    return int(txn);
}


} // namespace ce 
#endif // CE_WX