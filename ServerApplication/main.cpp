#include <iostream>
#include "CServer.h"

using namespace std;

void threadFun()
{
    //int * ptr = (int *) data;
    std::cout << "New Thread For Client Created" << std::endl;
    std::cout << std::this_thread::get_id() << std::endl;
}

void ClientSession(CSocket m_Socket)
{
    CLogger *pLogger;
    pLogger = CLogger::getLoggerInstance();

    std::cout << "Thread For New Client Created : " << std::this_thread::get_id() << std::endl;
    pLogger->LogWithTimeStamp("Hi From Client!");
    pLogger->Log("Number List From Client : ");
    while ( m_Socket.ClientSocket != INVALID_SOCKET )
    {
        std::vector<std::string> vRecvMsgBuff = m_Socket.msgReceive<std::string>(false);
        std::vector<std::string> vSendMsgBuff;
        std::cout << "Vector Size : " << vRecvMsgBuff.size() << std::endl;
        for (int i = 0; i < vRecvMsgBuff.size();i ++)
        {
            std::cout << vRecvMsgBuff[i] << std::endl;
            pLogger->Log(vRecvMsgBuff[i]);
            pLogger->Log(" ");
            int iNum = std::stoi(vRecvMsgBuff[i]);
            if ( iNum % 2 == 0 )
            {
                //m_Socket.msgSend((vRecvMsgBuff[i]).c_str(),false);
                std::cout << iNum << std::endl;
                //pLogger->Log(vRecvMsgBuff[i]);
                //pLogger->Log(" ");
                std::string sNumToBePushed = to_string(iNum);
                vSendMsgBuff.push_back(sNumToBePushed);
            }
        }
        pLogger->Log("\n");
        pLogger->Log("Number List To Client : ");
        for (int i = 0; i < vSendMsgBuff.size();i ++)
        {
            std::cout << "Message to be sent : " << vSendMsgBuff[i] << std::endl;
            std::string sNum = vSendMsgBuff[i];
            m_Socket.msgSend(sNum.c_str(),false);
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            std::cout << sNum << std::endl;
            pLogger->Log(vSendMsgBuff[i]);
            pLogger->Log(" ");
        }
        pLogger->Log("\n");
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

}

int main()
{
    CSocket m_Socket;
    m_Socket.setAddrInfoForServer();
    CSocket::initializeWinsock();
    std::vector<std::thread> threads;
    int i = 0;
    while(i<50)
    {
        m_Socket.getClietSocket();
        while ( !m_Socket.isValidClientSocket() )
        {

        }
        threads.emplace_back(ClientSession,m_Socket);
    }
    for (std::thread & t : threads) {
    t.join();
    }
    while(1){}
    cout << "EOP!" << endl;
    return 0;
}
