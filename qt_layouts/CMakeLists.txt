cmake_minimum_required(VERSION 3.5)

project(qt_layouts LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fPIC -O2")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -g -O0")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -fPIC -Wall -g -O0")

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-omit-frame-pointer -fsanitize=leak")
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fno-omit-frame-pointer -fsanitize=leak")

#SET(Qt5_DIR /home/heaven7/heaven7/tools/QT/5.14.2/5.14.2/gcc_64/lib/cmake/Qt5)

#Cmake command line too long
if(WIN32)
    SET(CMAKE_C_USE_RESPONSE_FILE_FOR_OBJECTS 1)
    SET(CMAKE_CXX_USE_RESPONSE_FILE_FOR_OBJECTS 1)

    SET(CMAKE_C_RESPONSE_FILE_LINK_FLAG "@")
    SET(CMAKE_CXX_RESPONSE_FILE_LINK_FLAG "@")
    #ninja
    SET(CMAKE_NINJA_FORCE_RESPONSE_FILE 1 CACHE INTERNAL "")
endif(WIN32)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

## handler os
set(HANDLER_OS_DIR D:/study/cpp/ENV/handler_os)
include_directories(${HANDLER_OS_DIR}/include)
set(HANDLER_LIB ${HANDLER_OS_DIR}/lib/libhandler_os.a pthread)

# qt
#find_package(Qt5 COMPONENTS Core Widgets REQUIRED)
find_package(QT NAMES Qt5 Qt6 REQUIRED COMPONENTS Core Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Widgets)
#set(QT_LIBS Qt5::Widgets)

set(QT_LIBS Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Widgets)
message(STATUS "QT_LIBS: ${QT_LIBS}")

## skia

set(SKIA_DIR D:/study/cpp/ENV/skia)
set(SKIA_INC ${SKIA_DIR})
find_library(LIB_SKIA skia expat gif jpeg-turbo png zlib HINTS "${SKIA_DIR}/lib")

add_library(skia STATIC IMPORTED GLOBAL)
set_target_properties(skia PROPERTIES IMPORTED_LOCATION
    ${LIB_SKIA}
)
# set_target_properties(skia PROPERTIES
#   IMPORTED_LOCATION "${LIB_MATH_RELEASE}"
#   IMPORTED_LOCATION_DEBUG "${LIB_MATH_DEBUG}"
#   IMPORTED_CONFIGURATIONS "RELEASE;DEBUG"
# )

# old
file(GLOB_RECURSE SRC_CORE qt_layouts/*.h
    qt_layouts/*.cpp
    qt_layouts/*.ui)

source_group(OLD_SRC FILES ${SRC_CORE})

add_library(qt_layouts
    ${SRC_CORE}
    )
target_link_libraries(qt_layouts PUBLIC ${QT_LIBS})

# view
file(GLOB_RECURSE SRC_CORE core/*.h
    core/*.cpp
    input/*.h
    input/*.cpp
    )
file(GLOB_RECURSE SRC_VIEW view/*.h
    view/*.cpp
    view/*.ui)
source_group(VIEW FILES ${SRC_CORE} ${SRC_VIEW})
add_library(qt_views
    ${SRC_CORE}
    ${SRC_VIEW}
    )
target_include_directories(qt_views PRIVATE ${SKIA_INC})
target_link_libraries(qt_views PUBLIC
    ${QT_LIBS}
    ${HANDLER_LIB}
    skia
    )

# test
file(GLOB_RECURSE SRC_TEST test/*.h
    test/*.cpp
    test/*.ui)

source_group(TESTS FILES ${SRC_TEST})
add_executable(test_qt_layouts
    ${SRC_TEST}
    )
target_link_libraries(test_qt_layouts PUBLIC
    qt_layouts
    qt_views
    ${QT_LIBS}
)
