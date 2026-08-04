cmake_minimum_required(VERSION 3.22)

project(study_vulkan LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
##
include(cmake/ENV.cmake)
# eigen
set(Eigen3_DIR ${EIGEN_DIR}/share/eigen3/cmake)
find_package(Eigen3)
set(Eigen_LIBS
    Eigen3::Eigen
    )
##
if(TI_WITH_VULKAN)
    add_compile_definitions(TI_WITH_VULKAN)
endif()
if(TI_WITH_CUDA)
    add_compile_definitions(TI_WITH_CUDA)
endif()
if(TI_ARCH_x64)
    add_compile_definitions(TI_ARCH_x64)
endif()
if(TI_WITH_LLVM)
    add_compile_definitions(TI_WITH_LLVM)
endif()
if(TI_WITH_GGUI)
    add_compile_definitions(TI_WITH_GGUI)
endif()

##
include_directories(./)
include_directories(external/)
include_directories(external/include)
include_directories(external/spdlog/include)
include_directories(external/VulkanMemoryAllocator/include)
include_directories(external/volk)
include_directories(external/PicoSHA2)
include_directories(external/FP16/include)
include_directories(external/SPIRV-Tools/include)
#
add_subdirectory(external/)
add_subdirectory(taichi)

##
add_executable(study_vulkan main.cpp)
