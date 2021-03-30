//
// Created by Administrator on 2021/3/30 0030.
//

#ifndef GLOBALUIAPP_AGG_EXTRA_H
#define GLOBALUIAPP_AGG_EXTRA_H

#include <stdlib.h>

#ifdef __cplusplus
extern "C"{
#endif

static unsigned int ScreenWidth = 600;
static unsigned int ScreenHeight = 600;
static char* _assetDir = NULL;

bool getFilePath(const char* p, char** out);

void sdl2_setup(const char* assertPath, int w, int h);

#ifdef __cplusplus
}
#endif


#endif //GLOBALUIAPP_AGG_EXTRA_H
