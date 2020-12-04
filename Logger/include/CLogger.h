#ifndef CLOGGER_H
#define CLOGGER_H

#include <iostream>
#include <string>
#include <ctime>
#include <fstream>

class CLogger
{
    public:

        virtual ~CLogger();

        static CLogger * getLoggerInstance();

        static CLogger *m_pSingletonInstanceOfLogger;

        static std::ofstream m_LogFile;

        static std::string m_sLogFileName;

        void Log(const std::string& sMessage);

        void LogWithTimeStamp(const std::string& sMessage);

        void setLogFileName ( std::string sLogFile);



    protected:

    private:

        CLogger();
};

#endif // CLOGGER_H
