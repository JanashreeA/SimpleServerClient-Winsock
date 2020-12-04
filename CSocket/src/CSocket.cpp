#include "CSocket.h"

CSocket::CSocket()
{
    ConnectSocket = INVALID_SOCKET;

    ClientSocket = INVALID_SOCKET;

    ListenSocket = INVALID_SOCKET;

    m_bWinSockInitializationStatus = true;

    m_pResultAddrInfo = NULL;

    ZeroMemory(&m_sAddrInfo, sizeof(m_sAddrInfo));

    //initializeWinsock();

    //setAddrInfo( ai_family,ai_socktype,ai_protocol,ai_flags = 0);
}


void CSocket::initializeWinsock()
{
    WSADATA wsadata;
    //m_bWinSockInitializationStatus = true;
    //Initialize WinSock
    std::cout << "Initializing Winsock..." << std::endl;
    if( WSAStartup(MAKEWORD(2,2),&wsadata) != 0 )
    {
        std::cout << "Failed! Error Code : " << WSAGetLastError();
        //m_bWinSockInitializationStatus = false;
    }
    std::cout << "Winsock Initialization Successful!" << std::endl;
}

void CSocket::setAddrInfo(int ai_family,int ai_socktype,int ai_protocol,int ai_flags)
{
    m_sAddrInfo.ai_family = ai_family;
    m_sAddrInfo.ai_socktype = ai_socktype;
    m_sAddrInfo.ai_protocol = ai_protocol;
    m_sAddrInfo.ai_flags = ai_flags;

}

void CSocket::setAddrInfoForClient(bool isTCP)
{
    int ai_family = AF_UNSPEC;
    int ai_socktype = SOCK_STREAM;
    int ai_protocol;
    if ( isTCP )
        ai_protocol = IPPROTO_TCP;
    else
        ai_protocol = IPPROTO_UDP;
    int ai_flags = 0;
    setAddrInfo( ai_family, ai_socktype, ai_protocol, ai_flags);
}

void CSocket::setAddrInfoForServer(bool isTCP)
{
    int ai_family = AF_INET;
    int ai_socktype = SOCK_STREAM;
    int ai_protocol;
    if ( isTCP )
        ai_protocol = IPPROTO_TCP;
    else
        ai_protocol = IPPROTO_UDP;
    int ai_flags = AI_PASSIVE;
    setAddrInfo( ai_family, ai_socktype, ai_protocol, ai_flags);
}

void CSocket::getResultAddrInfo()
{
    int iResult = getaddrinfo(NULL, DEFAULT_PORT, &m_sAddrInfo, &m_pResultAddrInfo);
    if ( iResult != 0)
    {
        std::cout << "Get Addr Info failed with error" << std::endl;
        WSACleanup();
        return;
    }
    std::cout << "Get Addr Info Successfull " << m_pResultAddrInfo->ai_family<<std::endl;
}

void CSocket::getConnectSocket()
{
    struct addrinfo *pAddrInfo = NULL;
    getResultAddrInfo();
    for ( pAddrInfo = m_pResultAddrInfo; pAddrInfo != NULL; pAddrInfo = pAddrInfo->ai_next)
    {
       ConnectSocket = socket(pAddrInfo->ai_family, pAddrInfo->ai_socktype, pAddrInfo->ai_protocol);
        if ( ConnectSocket == INVALID_SOCKET)
        {
            std::cout << "Socket Creation Failed due to " << WSAGetLastError() << std::endl;
            WSACleanup();
            return;
        }
        std::cout << "Socket Success"<< std::endl;
        //connect to server
        int iResult = connect(ConnectSocket,pAddrInfo->ai_addr,(int)pAddrInfo->ai_addrlen);
        if ( iResult == SOCKET_ERROR)
        {
            closesocket(ConnectSocket);
            ConnectSocket = INVALID_SOCKET;
            continue;
        }
        break;
    }
    if ( ConnectSocket == INVALID_SOCKET )
    {
        std::cout << "Unable to create Connect Socket" << std::endl;
        WSACleanup();
        return;
    }
    pAddrInfo = m_pResultAddrInfo;
    std::cout << "Connect Socket Created Successfully" << std::endl;
}

void CSocket::getListenSocket()
{
    getResultAddrInfo();
    ListenSocket = socket(m_pResultAddrInfo->ai_family, m_pResultAddrInfo->ai_socktype, m_pResultAddrInfo->ai_protocol);
    if ( ListenSocket == INVALID_SOCKET)
    {
        std::cout << "Listen Socket Creation Failed due to " << WSAGetLastError() << std::endl;
        freeaddrinfo(m_pResultAddrInfo);
        WSACleanup();
        return;
    }
    std::cout << "Listen Socket created Successfully " << std::endl;
}
bool CSocket::bindListenSocket()
{
    if ( ListenSocket != INVALID_SOCKET )
    {
        int iResult = bind( ListenSocket, m_pResultAddrInfo->ai_addr,(int)m_pResultAddrInfo->ai_addrlen );
        if ( iResult == SOCKET_ERROR)
        {
            std::cout << "Binding failed with error " << WSAGetLastError() << std::endl;
            WSACleanup();
            return false;
        }
        freeaddrinfo(m_pResultAddrInfo);
        std::cout << "Binding Successful" << std::endl;
        return true;
    }
    return false;
}

bool CSocket::Listen()
{
    if ( ListenSocket != INVALID_SOCKET )
    {
        if ( bindListenSocket())
        {
            int iResult = listen(ListenSocket,SOMAXCONN);
            if ( iResult == SOCKET_ERROR )
            {
                std::cout << "Listen Failed with error " << WSAGetLastError() << std::endl;
                closesocket(ListenSocket);
                WSACleanup();
                return false;
            }
            return true;
        }
    }
    return false;
}

void CSocket::getClietSocket()
{
    getListenSocket();
    if (Listen())
    {
        ClientSocket = accept (ListenSocket, NULL, NULL);
        if ( ClientSocket == INVALID_SOCKET)
        {
            std::cout << "Client Socket creation failed with error " << WSAGetLastError() << std::endl;
            closesocket(ListenSocket);
            WSACleanup();
            return;
        }
        std::cout << "Client Socket Created " << std::endl;
        closesocket(ListenSocket);
    }
}

bool CSocket::isValidConnectSocket()
{
    if ( ConnectSocket != INVALID_SOCKET )
    {
        return true;
    }
    return false;
}

bool CSocket::isValidClientSocket()
{
    if ( ClientSocket != INVALID_SOCKET )
    {
        return true;
    }
    return false;
}

void CSocket::msgSend(std::string msgBuff, bool isClient)
        {
            int iResult;
            if ( isClient)
            {
                iResult = send(ConnectSocket, msgBuff.c_str(), (int)strlen(msgBuff.c_str()),0);
            }
            else
            {
                iResult = send(ClientSocket, msgBuff.c_str(), (int)strlen(msgBuff.c_str()),0);
            }
            if ( iResult == SOCKET_ERROR )
            {
                std::cout << "Message sent failed with error " << WSAGetLastError() << std::endl;
                if (isClient )
                    closesocket(ConnectSocket);
                else
                    closesocket(ClientSocket);
                WSACleanup();
                return;
            }
            std::cout << "Bytes Sent " << iResult << std::endl;
        }

void CSocket::shutDown(bool isClient)
{
    int iResult;
    if ( isClient)
    {
        iResult = shutdown(ConnectSocket,SD_SEND);
    }
    else
    {
        iResult = shutdown(ClientSocket,SD_SEND);
    }
    if ( iResult == SOCKET_ERROR)
    {
        std::cout << "Shutdown failed with error " << WSAGetLastError() << std::endl;
        if (isClient )
            closesocket(ConnectSocket);
        else
            closesocket(ClientSocket);
        WSACleanup();
        return;
    }
    if (isClient )
        closesocket(ConnectSocket);
    else
        closesocket(ClientSocket);
    WSACleanup();
}

CSocket::~CSocket()
{

}
