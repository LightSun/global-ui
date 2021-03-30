//
// Created by Administrator on 2021/3/30 0030.
//

#include "stdlib.h"
#include <cstring>
#include "jni.h"
#include "JNIBridge.h"
#include "../agg-src/src/platform/sdl2/agg_platform_support.h"

extern "C"
JNIEXPORT void JNICALL
Java_com_heaven7_globalui_util_JNIBridge_nInit(JNIEnv *env, jclass cls, jstring str,jint w, jint h){
    const char *cstr = env->GetStringUTFChars(str, NULL);
    sdl2_setup(cstr, w, h);
    env->ReleaseStringUTFChars(str, cstr);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_heaven7_globalui_util_JNIBridge_nRelease(JNIEnv *env, jclass clazz) {
    if(_assetDir){
        free(_assetDir);
        _assetDir = NULL;
    }
}