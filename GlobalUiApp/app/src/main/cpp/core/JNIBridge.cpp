//
// Created by Administrator on 2021/3/30 0030.
//

#include "stdlib.h"
#include <cstring>
#include "jni.h"
#include "../agg-src/src/platform/sdl2/agg_platform_support.h"

extern "C"
JNIEXPORT void JNICALL
Java_com_heaven7_globalui_util_JNIBridge_nInit(JNIEnv *env, jclass cls, jstring str,jint w, jint h){
    Android_ScreenWidth = w;
    Android_ScreenHeight = h;
    const char *cstr = env->GetStringUTFChars(str, NULL);

    _assetDir = static_cast<char *>(malloc(strlen(cstr)+ 1));
    strcpy(_assetDir, cstr);
    env->ReleaseStringUTFChars(str, cstr);
    LOGD("_assetDir = %s", _assetDir);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_heaven7_globalui_util_JNIBridge_nRelease(JNIEnv *env, jclass clazz) {
    if(_assetDir){
        free(_assetDir);
        _assetDir = NULL;
    }
}