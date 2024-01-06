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

find_package(Qt5 COMPONENTS Core Widgets REQUIRED)
# core
file(GLOB_RECURSE SRC_CORE qt_layouts/*.h
    qt_layouts/*.cpp
    qt_layouts/*.ui)

add_library(qt_layouts
    ${SRC_CORE}
    )
target_link_libraries(qt_layouts Qt5::Widgets)

# view
file(GLOB_RECURSE SRC_VIEW view/*.h
    view/*.cpp
    view/*.ui)

add_library(qt_views
    ${SRC_VIEW}
    )
target_link_libraries(qt_views Qt5::Widgets)

# test
file(GLOB_RECURSE SRC_TEST test/*.h
    test/*.cpp
    test/*.ui)
add_executable(test_qt_layouts
    ${SRC_TEST}
    )
target_link_libraries(test_qt_layouts PUBLIC qt_layouts qt_views)
