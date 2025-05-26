// TimestampHelper.h
#ifndef TIMESTAMPHELPER_H
#define TIMESTAMPHELPER_H

#include <ctime>
#include <string>
using namespace std;


inline string currentTimestamp() {
    time_t     t     = time(NULL);
    struct tm* tm_i  = localtime(&t);
    char       buf[20];
    // formats into YYYY-MM-DD HH:MM:SS
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", tm_i);
    return string(buf);
}

#endif // TIMESTAMPHELPER_H

