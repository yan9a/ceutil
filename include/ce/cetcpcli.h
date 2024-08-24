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
#include <cstdio>
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
/////////////////////////////////////////////////////////////////////////////
// Implementation

inline void ceTcpClient::PrintLog(std::string str)
{
    // printf("%s \n",str.c_str());
}

inline ceTcpClient::ceTcpClient(wxEvtHandler* app, int socketid) :
    _app(app), _socket_id(socketid)
{
    // Create the socket
    this->_socketClient = new wxSocketClient();
    // Setup the event handler and subscribe to most events
    _socketClient->SetEventHandler(*this, _socket_id);
    _socketClient->SetNotify(wxSOCKET_CONNECTION_FLAG |
        wxSOCKET_INPUT_FLAG |
        wxSOCKET_LOST_FLAG);
    _socketClient->Notify(true);
    Connect(_socket_id, wxEVT_SOCKET, wxSocketEventHandler(ceTcpClient::OnSocketEvent));
}

inline ceTcpClient::~ceTcpClient()
{
    this->Close();
    Disconnect(_socket_id, wxEVT_SOCKET);
    _socketClient->Destroy();
}

inline void ceTcpClient::SetRemote(std::string remotehost, int port)
{
    _remotehost = remotehost;
    _port = port;
}

inline void ceTcpClient::SetRemotehost(std::string remotehost)
{
    _remotehost = remotehost;
}

inline void ceTcpClient::SetPort(int port)
{
    _port = port;
}

inline std::string ceTcpClient::GetRemotehost()
{
    return _remotehost;
}

inline int ceTcpClient::GetPort()
{
    return _port;
}

inline void ceTcpClient::Open()
{
    // Create the address
    IPaddress addr;
    //addr.AnyAddress();
    addr.Hostname(_remotehost);
    addr.Service(_port);
    PrintLog("Trying to connect to " + addr.IPAddress().ToStdString()
        + ":" + std::to_string(addr.Service()));

    // we connect asynchronously and will get a wxSOCKET_CONNECTION event when
    // the connection is really established
    //
    // if you want to make sure that connection is established right here you
    // could call WaitOnConnect(timeout) instead

    _socketClient->Connect(addr, false);
}

inline void ceTcpClient::Close()
{
    _socketClient->Close();
}

inline void ceTcpClient::OnSocketEvent(wxSocketEvent& event)
{
    PrintLog("OnSocketEvent");
    wxSocketBase* sockBase = event.GetSocket();
    std::vector<char> vc;
    char buf[CE_TCPCLIENT_RX_BUF_SIZE];

    // First, print a message
    switch (event.GetSocketEvent())
    {
    case wxSOCKET_INPUT:
        this->PrintLog("wxSOCKET_INPUT");
        break;
    case wxSOCKET_LOST:
        this->PrintLog("wxSOCKET_LOST");
        break;
    case wxSOCKET_CONNECTION:
        this->PrintLog("wxSOCKET_CONNECTION");
        break;
    default:
        this->PrintLog("Unexpected event");
        break;
    }

    // Now we process the event
    switch (event.GetSocketEvent())
    {
    case wxSOCKET_INPUT:
    {
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
        if (!lenRd) {
            PrintLog("Failed to read message");
            return;
        }
        else {
            PrintLog("Read " + std::to_string(lenRd) + " bytes");
            // string str(buf,lenRd);
            // PrintLog("Rx: " + str)
        }
        // Enable input events again.
        sockBase->SetNotify(wxSOCKET_LOST_FLAG | wxSOCKET_INPUT_FLAG);
        vc.assign(buf, buf + lenRd);
        wxThreadEvent eventskt(wxEVT_THREAD, _socket_id);
        eventskt.SetPayload<std::vector<char>>(vc);
        eventskt.SetString(_remotehost);
        eventskt.SetInt(_port);
        wxQueueEvent(_app, eventskt.Clone());
        break;
    }
    default:
        break;
    }
}

inline int ceTcpClient::Tx(std::vector<char> bv)
{
    size_t txn = 0;
    if (this->_socketClient->IsOk() && this->_socketClient->IsConnected()) {
        // Write to pointed client
        txn = bv.size();
        size_t n = _socketClient->Write(bv.data(), txn).LastCount();
        if (n != txn) {
            perror("ceTcpClient write error");
            txn = n;
        }
        else {
            PrintLog("Tx: " + ceMisc::cvec2hex(bv));
        }
    }
    return int(txn);
}

inline bool ceTcpClient::IsConnected()
{
    return this->_socketClient->IsConnected();
}

inline bool ceTcpClient::IsOK()
{
    return this->_socketClient->IsOk();
}

/////////////////////////////////////////////////////////////////////////////
} // namespace ce 
#endif // CE_WX
#endif // CETCPCLIENT_H
