cmake_minimum_required(VERSION 3.4.1)
project(globalApp)

set(LIB_DIR ${CMAKE_SOURCE_DIR}/src/main/jniLibs/${ANDROID_ABI})
include_directories(src/main/cpp/include)
include_directories(src/main/cpp/agg-src/include)
#sdl2 (2.0.14)
add_library(sdl2 SHARED IMPORTED)
set_target_properties(sdl2 PROPERTIES IMPORTED_LOCATION ${LIB_DIR}/libSDL2.so)

# agg
set(DIR_AGG ${CMAKE_SOURCE_DIR}/src/main/cpp/agg-src)
include_directories(${DIR_AGG}/agg2d)
include_directories(${DIR_AGG}/font_freetype)
aux_source_directory(${DIR_AGG}/src SRC_AGG)
aux_source_directory(${DIR_AGG}/src/ctrl SRC_AGG)
aux_source_directory(${DIR_AGG}/src/platform/sdl2 SRC_AGG)
aux_source_directory(${DIR_AGG}/agg2d SRC_AGG)
#aux_source_directory(${DIR_AGG}/font_freetype SRC_AGG)

#freetype
set(DIR_HALFBUZZ_FREETYPE src/main/cpp/harfbuzz-icu-freetype)
include_directories(${DIR_HALFBUZZ_FREETYPE}/freetype/include)
include_directories(${DIR_HALFBUZZ_FREETYPE}/harfbuzz/src)
include_directories(${DIR_HALFBUZZ_FREETYPE}/harfbuzz-generated)
include_directories(${DIR_HALFBUZZ_FREETYPE}/icu/common)
add_subdirectory(${DIR_HALFBUZZ_FREETYPE} harfbuzz_out)

add_library(
        agg
        SHARED
        ${SRC_AGG}
        ${DIR_AGG}/font_freetype/agg_font_freetype.cpp
        src/main/cpp/core/JNIBridge.cpp
       # ${DIR_AGG}/examples/agg2d_demo.cpp
        ${DIR_AGG}/examples/freetype_test.cpp
        ${DIR_AGG}/font_freetype/agg_font_freetype.cpp
)

set_target_properties(agg PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(
        agg
        sdl2
        #harfbuzz
        freetype
        #icucommon
        dl
        GLESv1_CM
        GLESv2
        OpenSLES
        log
        android
)