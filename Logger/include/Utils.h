#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

#include <string>
#include <time.h>

namespace utils
{
    std::string getTimeStamp()
    {
        time_t     now = time(NULL);
        struct tm  tstruct;
        char buf[80];
        localtime_s(&tstruct, &now);
        strftime(buf, 80, "%Y-%m-%d.%X", &tstruct);
        return buf;
    }
}

#endif // UTILS_H_INCLUDED
