#include "CServer.h"
#include <process.h>
CServer::CServer()
{
    m_Socket.initializeWinsock();
    m_Socket.setAddrInfoForServer();
}


void CServer::connectionHandler()
{
    m_Socket.getClietSocket();
}

void CServer::msgSend()
{
    m_Socket.msgSend("\nHello From Server",false);
}

void CServer::msgReceive()
{
    std::vector<std::string> vRecvMsgBuff = m_Socket.msgReceive<std::string>(false);
    for (auto itr : vRecvMsgBuff)
    {
        std::cout << itr << std::endl;
    }
}

CServer::~CServer()
{
}
