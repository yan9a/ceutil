/////////////////////////////////////////////////////////////////////////////
// Name:         ceTcpClient.cpp
// Description:  TCP client module
// Author:       Yan Naing Aye
// Ref: http://cool-emerald.blogspot.com/2018/01/udptcp-socket-programming-with-wxwidgets.html
/////////////////////////////////////////////////////////////////////////////
#include <cstdio>
#include "ce/ceTcpClient.h"
using namespace std;

#if CE_WX==1
namespace ce {

void ceTcpClient::PrintLog(string str)
{
    // printf("%s \n",str.c_str());
}

ceTcpClient::ceTcpClient(wxAppConsole* app, int socketid):
_app(app),_socket_id(socketid)
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

ceTcpClient::~ceTcpClient()
{
    this->Close();
    Disconnect(_socket_id, wxEVT_SOCKET);
    _socketClient->Destroy();
}

void ceTcpClient::SetRemote(string remotehost, int port)
{
    _remotehost = remotehost;
    _port = port;
}

void ceTcpClient::Open()
{
    // Create the address
    IPaddress addr;
    //addr.AnyAddress();
    addr.Hostname(_remotehost);
    addr.Service(_port);
    PrintLog("Trying to connect to " + addr.IPAddress().ToStdString() 
        + ":" + to_string(addr.Service()));

    // we connect asynchronously and will get a wxSOCKET_CONNECTION event when
    // the connection is really established
    //
    // if you want to make sure that connection is established right here you
    // could call WaitOnConnect(timeout) instead

    _socketClient->Connect(addr, false);
}

void ceTcpClient::Close()
{
    _socketClient->Close();
}

void ceTcpClient::OnSocketEvent(wxSocketEvent& event)
{
	PrintLog("OnSocketEvent");
	wxSocketBase *sockBase = event.GetSocket(); 
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
                PrintLog("Read " + to_string(lenRd) + " bytes");
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

int ceTcpClient::Tx(std::vector<char> bv)
{
    size_t txn = 0;
    if (this->_socketClient->IsOk() && this->_socketClient->IsConnected()) {
        // Write to pointed client
        txn = bv.size();
        size_t n = _socketClient->Write(bv.data(), txn).LastCount();
        if ( n!= txn) {
            perror("ceTcpClient write error");
            txn = n;
        }
        else {
            PrintLog("Tx: " + ceMisc::cvec2hex(bv));
        }        
    }
    return int(txn);
}

bool ceTcpClient::IsConnected()
{
    return this->_socketClient->IsConnected();
}

bool ceTcpClient::IsOK()
{
    return this->_socketClient->IsOk();
}

} // namespace ce 
#endif // CE_WX