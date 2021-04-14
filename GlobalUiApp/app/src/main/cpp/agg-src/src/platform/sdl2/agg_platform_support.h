//
// Created by Administrator on 2021/3/30 0030.
//

#ifndef GLOBALUIAPP_AGG_PLATFORM_SUPPORT_H
#define GLOBALUIAPP_AGG_PLATFORM_SUPPORT_H

#include <stdlib.h>
#include "agg_extra.h"

#if __ANDROID__
#ifdef NATIVE_LOG
#define LOG_TAG "NATIVE_LOG"

#include <android/log.h>

#define LOGV(...) __android_log_print(ANDROID_LOG_VERBOSE,LOG_TAG,__VA_ARGS__)
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN,LOG_TAG,__VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
#define LOGF(...) __android_log_print(ANDROID_LOG_FATAL,LOG_TAG,__VA_ARGS__)
#else
#define LOGV(...) do{}while(0)
#define LOGD(...) do{}while(0)
#define LOGI(...) do{}while(0)
#define LOGW(...) do{}while(0)
#define LOGE(...) do{}while(0)
#define LOGF(...) do{}while(0)
#endif
#define DEBUG_PRINT(...) LOGD(__VA_ARGS__)
#define ERROR_PRINT(...) LOGE(__VA_ARGS__)
#else //!__ANDROID__
#define DEBUG_PRINT(...) do{fprintf(stderr, __VA_ARGS__ ); } while (false)
#define ERROR_PRINT(...) do{fprintf(stderr, __VA_ARGS__ ); } while (false)
#endif

#define sdl2_do_file(p, x, ends) \
do{ \
char *_out;\
bool _malloced = getFilePath(p, &_out);\
LOGV("sdl2_do_file: %s", _out);\
x;\
if(_malloced) free(_out);\
}while(0);\
ends;

#endif //GLOBALUIAPP_AGG_PLATFORM_SUPPORT_H
