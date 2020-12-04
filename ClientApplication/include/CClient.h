#ifndef CCLIENT_H
#define CCLIENT_H

#include <iostream>
#include "CSocket.h"
#include "CLogger.h"
#include <string>
#include <vector>
#include <chrono>
#include <thread>

class CClient
{
    public:
        CClient();

        ~CClient();

        CSocket m_Socket;

        void connectionHandler();

        void msgSend(std::string sMessage);

        void msgReceive();

        void shutDown();

        bool m_bSocketCreationStatus;

    protected:

    private:
};

#endif // CCLIENT_H
