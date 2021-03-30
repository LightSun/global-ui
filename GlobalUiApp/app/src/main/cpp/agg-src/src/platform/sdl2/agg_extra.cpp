//
// Created by Administrator on 2021/3/30 0030.
//

#include <stdlib.h>
#include <string.h>
#include "agg_platform_support.h"

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