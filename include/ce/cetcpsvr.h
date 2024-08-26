/////////////////////////////////////////////////////////////////////////////
// Name:         cetcpsvr.h
// Description:  TCP server module
// Author:       Yan Naing Aye
// Ref: http://cool-emerald.blogspot.com/2018/01/udptcp-socket-programming-with-wxwidgets.html
/////////////////////////////////////////////////////////////////////////////
// Requires: wxWidgets
/////////////////////////////////////////////////////////////////////////////

#ifndef CETCPSVR_H
#define CETCPSVR_H

#define CE_WX 1 // include WX
#define CE_DBG_PRINT 0 // print dbg mes

	#ifndef CE_MACROS_H
		#define CE_MACROS_H


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
#include <iostream>
#include <string>
#include <deque>
#include <vector>
#include <utility>
#include <algorithm>
#include "wx/wx.h"
#include "wx/socket.h"
// this example is currently written to use only IP or only IPv4 sockets, it
// should be extended to allow using either in the future
//#if wxUSE_IPV6
//typedef wxIPV6address IPaddress;
//#else
typedef wxIPV4address IPaddress;
//#endif
#define CE_DEFAULE_TCPSVR_RX_BUF_SIZE 32768

namespace ce {
class ceTcpServer : public wxEvtHandler {
public:
	// port = listening port number, max number of sockets
	// event will raise with id range = socketid - (socketid+ns-1)
	ceTcpServer(wxEvtHandler* eh, int serverid, int socketid, int port, int ns); 

    ~ceTcpServer();
	
	bool Open(); // return true = success, false = error
	bool Close();

    // Transmit (write) 
    // to all connected sockets
    int Tx(std::vector<char> bv); // return 0 = success, 1 = error

    // to the socket with id
    int Tx(int id, std::vector<char> bv); // return 0 = success, 1 = error

    // to all except the socket with id
    int TxExcept(int id, std::vector<char> bv); // return 0 = success, 1 = error

    // Receive (read) will raise receive event

	void SetBufSize(int n);
	int GetBufSize();

private:
	void OnServerEvent(wxSocketEvent& event);
    void OnSocketEvent(wxSocketEvent& event);
	wxEvtHandler* _eh;
	int _socket_id;
    int _server_id;
	int _port;
	bool _listening{false};
	int _ns{ 0 };
    wxSocketServer* _socketServer{};
    std::deque<std::pair<int,wxSocketBase*>> _socketBaseQ;
	void PrintLog(std::string str);
	int _bufSize{ CE_DEFAULE_TCPSVR_RX_BUF_SIZE };
	int GetFreeSocketId();
};

/////////////////////////////////////////////////////////////////////////////
// Implementation

inline void ceTcpServer::PrintLog(std::string str)
{
#if CE_DBG_PRINT==1
    std::cout << str << std::endl;
#endif
}

// port = listening port number, max number of sockets
// event will raise with id range = socketid - (socketid+ns-1)
inline ceTcpServer::ceTcpServer(wxEvtHandler* eh, int serverid, int socketid, int port, int ns) :
    _eh(eh), _server_id(serverid), _socket_id(socketid), _port(port), _ns(ns)
{
    Connect(serverid, wxEVT_SOCKET, wxSocketEventHandler(ceTcpServer::OnServerEvent));
    for (int i = 0; i < ns; i++) {
        Connect(socketid + i, wxEVT_SOCKET, wxSocketEventHandler(ceTcpServer::OnSocketEvent));
    }
}

inline ceTcpServer::~ceTcpServer()
{
    this->Close();
    for (int i = 0; i < _ns; i++) {
        Disconnect(_socket_id + i, wxEVT_SOCKET);
    }
    Disconnect(_server_id, wxEVT_SOCKET);
}

inline bool ceTcpServer::Open()
{
    if (this->_listening) { return true; } // already listening

    // Create the address - defaults to localhost:0 initially
    IPaddress addr;
    addr.AnyAddress();
    addr.Service(this->_port);
    PrintLog("Creating server at " + addr.IPAddress().ToStdString() + ":" + std::to_string(addr.Service()));

    // Create the socket
    this->_socketServer = new wxSocketServer(addr);

    // We use IsOk() here to see if the server is really listening
    if (!this->_socketServer->IsOk()) {
        perror("ceTcpServer could not listen at the specified port");
        return false;
    }

    IPaddress addrReal;
    if (!this->_socketServer->GetLocal(addrReal)) {
        perror("ceTcpServer couldn't get the address it bounds to");
    }
    else {
        PrintLog("Server listening at " + addrReal.IPAddress().ToStdString() + ":" + std::to_string(addrReal.Service()));
    }

    // Setup the event handler and subscribe to connection events
    this->_socketServer->SetEventHandler(*this, _server_id);
    this->_socketServer->SetNotify(wxSOCKET_CONNECTION_FLAG);
    this->_socketServer->Notify(true);
    this->_listening = true;
    return true;
}

inline bool ceTcpServer::Close()
{
    if (!this->_listening) { return true; } // already closed
    for (auto& a : _socketBaseQ) {
        if (a.second != nullptr) {
            a.second->Destroy();
            a.second = nullptr;
        }
    }
    _socketBaseQ.clear();

    if (_socketServer->IsClosed()) {
        return true;
    }
    _socketServer->Discard();
    return _socketServer->Destroy();
}

inline int ceTcpServer::GetFreeSocketId()
{
    int id = _socket_id;
    for (int i = 0; i < _ns; i++) {
        bool f = true;
        for (const auto& a : _socketBaseQ) {
            if (a.first == (id+i)) {
                f = false;
                break;
            }
        }
        if (f) {
            id += i;
            break;
        }
    }
    // will happen to give base id, if all are used
    PrintLog("Found free socket id: " + std::to_string(id));
    return id;
}

inline void ceTcpServer::OnServerEvent(wxSocketEvent& event)
{
    PrintLog("OnServerEvent");
    wxSocketBase* sockBase;
    IPaddress addr;

    switch (event.GetSocketEvent())
    {
    case wxSOCKET_CONNECTION:
        PrintLog("Connection event");
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
            PrintLog("New client connection from " + addr.IPAddress().ToStdString() + ":" + std::to_string(addr.Service()));
        }
    }
    else
    {
        PrintLog("Couldn't accept a new connection");
        return;
    }

    if (this->_socketBaseQ.size() >= _ns) {
        // first in first out        
        auto a = this->_socketBaseQ.front();
        wxSocketBase* sockBase = a.second;
        if (sockBase != nullptr) {
            sockBase->Destroy();
        }
        PrintLog("Deleting front socket id: " + std::to_string(a.first));
        this->_socketBaseQ.pop_front();
    }

    int id = this->GetFreeSocketId();
    sockBase->SetEventHandler(*this, id);
    sockBase->SetNotify(wxSOCKET_INPUT_FLAG | wxSOCKET_LOST_FLAG);
    sockBase->Notify(true);
    this->_socketBaseQ.push_back(std::make_pair(id,sockBase));
    PrintLog(std::to_string(_socketBaseQ.size()) + " clients connected");

    std::string ip = addr.IPAddress().ToStdString();
    int port = addr.Service();

    // 1st char: flag 1=new con, 0 = lost con, 
    // 2nd char: number of con
    // 3rd char: relative socket id
    wxString ws(ip);
    wxThreadEvent eventsvr(wxEVT_THREAD, _server_id);
    std::vector<char> bv{ 1,(char)_socketBaseQ.size(), (char)(id-_socket_id)};
    eventsvr.SetPayload<std::vector<char>>(bv);
    eventsvr.SetString(ws);
    eventsvr.SetInt(port);
    wxQueueEvent(_eh, eventsvr.Clone());
}

inline void ceTcpServer::OnSocketEvent(wxSocketEvent& event)
{
    char* buf = new char[_bufSize];// dynamically allocate buf

    int id = event.GetId();
    PrintLog("OnSocketEvent id: "+std::to_string(id));
    wxSocketBase* sockBase = event.GetSocket();
    IPaddress addr;
    std::vector<char> vc;


    if (!sockBase->GetPeer(addr))
    {
        PrintLog("Socket event from unknown client");
    }
    else
    {
        PrintLog("Socket event from " + addr.IPAddress().ToStdString() + ":" + std::to_string(addr.Service()));
    }
    std::string ip = addr.IPAddress().ToStdString();
    int port = addr.Service();
    wxString ws(ip);

    switch (event.GetSocketEvent())
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
        size_t lenRd = sockBase->Read(buf, _bufSize).LastCount();
        if (!lenRd) {
            PrintLog("Failed to read message");
            return;
        }
        else {
            PrintLog("Read " + std::to_string(lenRd) + " bytes");
        }
        // Enable input events again.
        sockBase->SetNotify(wxSOCKET_LOST_FLAG | wxSOCKET_INPUT_FLAG);

        // string str(buf,lenRd);
        // PrintLog("Rx: " + str);

        vc.assign(buf, buf + lenRd);
        wxThreadEvent eventskt(wxEVT_THREAD, id);
        eventskt.SetPayload<std::vector<char>>(vc);
        eventskt.SetString(ws);
        eventskt.SetInt(port);
        wxQueueEvent(_eh, eventskt.Clone());
    }
    break;
    case wxSOCKET_LOST:
    {
        PrintLog("Lost socket id: " + std::to_string(id));
        auto it = std::find_if(_socketBaseQ.begin(), _socketBaseQ.end(),
            [id](const std::pair<int, wxSocketBase*>& element) {
                return element.first == id;
            });

        // Check if the element was found
        if (it != _socketBaseQ.end()) {
            PrintLog("Socket id removing from the list");
            // destroy at the end of method
            _socketBaseQ.erase(it);
        }
        else {
            PrintLog("Socket id not found in the list");
        }

        std::vector<char> bv{ 0,(char)this->_socketBaseQ.size(), char(id - _socket_id)};
        // 1st char: flag 1=new con, 0 = lost con, 
        // 2nd char: number of con
        // 3rd char: relative socket id

        wxThreadEvent eventsvr(wxEVT_THREAD, _server_id);
        eventsvr.SetPayload<std::vector<char>>(bv);
        eventsvr.SetString(ws);
        eventsvr.SetInt(port);
        wxQueueEvent(_eh, eventsvr.Clone());
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
        PrintLog("Remaining clients: " + std::to_string(this->_socketBaseQ.size()));
    }
    break;
    default:
        perror("Unexpected event");
        break;
    }
    delete[] buf; // destroy buf
}

inline int ceTcpServer::Tx(std::vector<char> bv)
{
    int r = 0;
    size_t txn = 0;    
    wxSocketBase* socketBase;
    for (auto& a : _socketBaseQ)
    {        
        socketBase = a.second;
        if (socketBase != nullptr) {
            if (socketBase->IsOk() && socketBase->IsConnected()) {
                // Write to pointed client
                txn = bv.size();
                size_t n = socketBase->Write(bv.data(), txn).LastCount();
                if (n != txn) {
                    perror("ceTcpClient write error");
                    r = 1;
                }
                else {
                    // PrintLog("Tx: " + ceMisc::cvec2hex(bv));
                }
            }
            else {
                r = 1;
            }
        }
    }
    return r;
}

inline int ceTcpServer::Tx(int id, std::vector<char> bv)
{
    int r = 0;
    size_t txn = 0;
    wxSocketBase* socketBase;
    auto it = std::find_if(_socketBaseQ.begin(), _socketBaseQ.end(),
        [id](const std::pair<int, wxSocketBase*>& element) {
            return element.first == id;
        });

    // Check if the element was found
    if (it != _socketBaseQ.end()) {
        socketBase = it->second;
        if (socketBase != nullptr) {
            if (socketBase->IsOk() && socketBase->IsConnected()) {
                // Write to pointed client
                txn = bv.size();
                size_t n = socketBase->Write(bv.data(), txn).LastCount();
                if (n != txn) {
                    perror("ceTcpClient write error");
                    r = 1;
                }
                else {
                    // PrintLog("Tx: " + ceMisc::cvec2hex(bv));
                }
            }
            else {
                r = 1;
            }
        }
    }
    else {
        PrintLog("Socket id not found in the list");
    }
    return r;
}

inline int ceTcpServer::TxExcept(int id, std::vector<char> bv)
{
    int r = 0;
    size_t txn = 0;
    wxSocketBase* socketBase;
    for (auto& a : _socketBaseQ)
    {
        if (a.first != id) {
            socketBase = a.second;
            if (socketBase != nullptr) {
                if (socketBase->IsOk() && socketBase->IsConnected()) {
                    // Write to pointed client
                    txn = bv.size();
                    size_t n = socketBase->Write(bv.data(), txn).LastCount();
                    if (n != txn) {
                        perror("ceTcpClient write error");
                        r = 1;
                    }
                    else {
                        // PrintLog("Tx: " + ceMisc::cvec2hex(bv));
                    }
                }
                else {
                    r = 1;
                }
            }
        }        
    }
    return r;
}

inline void ceTcpServer::SetBufSize(int n)
{
    this->_bufSize = n;
}

inline int ceTcpServer::GetBufSize()
{
    return _bufSize;
}

/////////////////////////////////////////////////////////////////////////////

} // namespace ce 
#endif // CE_WX
#endif // CETCPSVR_H
