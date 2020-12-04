#include "CLogger.h"
#include "Utils.h"

CLogger * CLogger::m_pSingletonInstanceOfLogger = 0;

std::ofstream CLogger::m_LogFile;

std::string CLogger::m_sLogFileName = "Log.txt";

CLogger::CLogger()
{

}

CLogger * CLogger::getLoggerInstance()
{
    if ( m_pSingletonInstanceOfLogger == NULL)
    {
        m_pSingletonInstanceOfLogger = new CLogger();
        m_LogFile.open(m_sLogFileName.c_str(),std::ios::out | std::ios::app);
        std::cout << "New Logger instance Created" << std::endl;
    }
    return m_pSingletonInstanceOfLogger;
}

void CLogger::setLogFileName ( std::string sLogFile)
{
    m_sLogFileName = sLogFile;
}

void CLogger::LogWithTimeStamp(const std::string& sMessage)
{
    m_LogFile << "          ";
    m_LogFile << utils::getTimeStamp() << "          ";
    m_LogFile << sMessage << " ";
    m_LogFile <<std::endl;
}

void CLogger::Log(const std::string& sMessage)
{
    m_LogFile << " ";
    m_LogFile << sMessage << " ";
}

CLogger::~CLogger()
{
    //dtor
}
