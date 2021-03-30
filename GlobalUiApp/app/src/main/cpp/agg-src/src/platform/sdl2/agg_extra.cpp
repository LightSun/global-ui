//
// Created by Administrator on 2021/3/30 0030.
//

#include <stdlib.h>
#include <string.h>
#include "agg_platform_support.h"

void sdl2_setup(const char* assertPath, int w, int h){
    ScreenWidth = static_cast<unsigned int>(w);
    ScreenHeight = static_cast<unsigned int>(h);

    _assetDir = static_cast<char *>(malloc(strlen(assertPath)+ 1));
    strcpy(_assetDir, assertPath);
    LOGD("sdl_jnibridge_setup: _assetDir = %s, w= %d, h = %d", _assetDir, w, h);
}

extern "C" bool getFilePath(const char* p, char** out){
    LOGD("getFilePath: _assetDir = %s", _assetDir);
    if(_assetDir){
        int len_dir = strlen(_assetDir);
        char* arr = static_cast<char *>(malloc(strlen(p) + len_dir + 2)); // '/' and '\0'
        *out = arr;
        strcpy(arr, _assetDir);
        strcpy(arr + len_dir, "/");
        strcpy(arr + len_dir + 1, p);
        arr[strlen(p) + len_dir + 1] = '\0';
        return true;
    }
    *out = const_cast<char *>(p);
    return false;
}