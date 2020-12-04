#ifndef CSERVER_H
#define CSERVER_H

#include <iostream>
#include "CSocket.h"
#include "CLogger.h"
#include <vector>
#include <string>
#include <chrono>
#include <thread>

class CServer
{
    public:
        CServer();

        ~CServer();

        CSocket m_Socket;

        void connectionHandler();

        void msgSend();

        void msgReceive();

        bool m_bSocketCreationStatus;

    protected:

    private:

};

#endif // CSERVER_H
