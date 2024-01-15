#pragma once

#include <string>
#include <memory>
#include <stdlib.h>

#define FUNC_NEW_SHARED(t)\
    template <typename ...Args>\
    static std::shared_ptr<t> New(Args&& ... args){\
        return std::make_shared<t>(std::forward<Args>(args)...);\
    }

#define FUNC_LOCAL_INS(T)\
    template <typename ...Args>\
    static T of(Args&& ... args){\
        return T(std::forward<Args>(args)...);\
    }

#define PRINTLN(fmt, ...) printf(fmt "\n", ##__VA_ARGS__)
#define ASSERT(con) if(!(con)) abort();

#define ASSERT_FMT(con, fmt, ...) \
    if(!(con)){\
        fprintf(stderr, fmt "\n", ##__VA_ARGS__);\
        abort();\
    }
