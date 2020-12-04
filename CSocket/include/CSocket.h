#ifndef CSOCKET_H
#define CSOCKET_H

#define _WINNT_WIN32 0x0601

#include <winsock2.h>   // Add Linker option ws2_32
#include <iostream>
#include <ws2tcpip.h>
#include <vector>
#include <string>
#include <cstring>

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"

class CSocket
{
    public:
        CSocket();

        ~CSocket();

        static void initializeWinsock();

        void setAddrInfoForClient(bool isTCP = true);

        void setAddrInfoForServer(bool isTCP = true);

        void setAddrInfo(int ai_flags,int ai_family,int ai_socktype,int ai_protocol);

        void getResultAddrInfo();

        void getConnectSocket();

        void getListenSocket();

        bool bindListenSocket();

        bool Listen();

        void getClietSocket();

        bool isValidConnectSocket();

        bool isValidClientSocket();

        void msgSend(std::string msgBuff, bool isClient = true);

        template<typename T>
        std::vector<T> msgReceive(bool isClient = true)
        {
            int iResult;
            int iMsgBuffLen = DEFAULT_BUFLEN;
            std::vector<T> vMsgBuff;
            char cMsgBuff[DEFAULT_BUFLEN];
            //do
            //{
                if ( isClient )
                {
                    iResult = recv(ConnectSocket, cMsgBuff, iMsgBuffLen,0);
                    std::cout << "Received Message From Server" << std::endl;
                }
                else
                {
                    iResult = recv(ClientSocket, cMsgBuff, iMsgBuffLen,0);
                    std::cout << "Received Message From Client" << std::endl;
                }
                if ( iResult > 0 )
                {
                    vMsgBuff.push_back(cMsgBuff);
                    std::cout << "Bytes Received " << iResult << std::endl;
                }
                else if ( iResult == 0 )
                {
                    std::cout << "Connection closing... "  << std::endl;
                }
                else
                {
                    std::cout << "Recv failed with error " << WSAGetLastError() << std::endl;
                    if ( isClient )
                        closesocket(ConnectSocket);
                    else
                    {
                        closesocket(ClientSocket);
                    }
                    std::cout << "Socket is Closed" << std::endl;
                    ClientSocket = INVALID_SOCKET;
                    WSACleanup();
                }
            //}while( iResult > 0);
            return vMsgBuff;
        }

        void shutDown(bool isClient = true);

        SOCKET ConnectSocket;

        SOCKET ListenSocket;

        SOCKET ClientSocket;

        bool m_bWinSockInitializationStatus;

        struct addrinfo m_sAddrInfo;

        struct addrinfo *m_pResultAddrInfo;

    protected:

    private:
};

#endif // CSOCKET_H
