#pragma once

#ifdef _WIN32
//#include <time.h>
#else
//#include <sys/time.h>
//#include <unistd.h>
#endif
#include <iostream>
#include <chrono>
#include <cstdio>

namespace System{

    using LLong = long long;

//in ms
static inline LLong currentMills(){
//#ifdef _WIN32
//    return clock();
//#else
    auto now = std::chrono::system_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(
                now.time_since_epoch()).count();
//    struct timeval time;
//    gettimeofday(&time, NULL);
//    return time.tv_usec/1000;
//#endif
}

//
static inline std::string formatTime(long long timeMs){
    //printf("formatTime >> timeMs + %llu\n", timeMs);
    time_t tt = timeMs / 1000;
    //auto localTm = gmtime(&tt);
    auto localTm = localtime(&tt);

    char buff[64];
    int len = strftime(buff, 64, "%Y-%m-%d %H:%M:%S", localTm);
    snprintf(buff + len, 64 - len, ".%03u", (unsigned int)(timeMs % 1000));
    return std::string(buff);
}

static inline std::string currentMillsStr(){
    return formatTime(currentMills());
}

}
