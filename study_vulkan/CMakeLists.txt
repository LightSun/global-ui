cmake_minimum_required(VERSION 3.22)

project(study_vulkan LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# 设置 -march=nehalem 后，编译器会启用比默认基准（x86-64）更多的指令集
#       SSE4.2（流式 SIMD 扩展 4.2）
#       POPCNT（快速计算二进制中 1 的个数）
#       SSE4.1、SSSE3、SSE3 等更早的指令集
#       同时也具备 CX16（128 位原子比较交换）支持
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=nehalem")

# -static-libgcc -static-libstdc++
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
#set(LLVM_ENABLE_ABI_BREAKING_CHECKS FORCE_OFF CACHE STRING "Force OFF" FORCE)

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
if (TI_WITH_DX12)
    add_compile_definitions(TI_WITH_DX12)
endif()
if (TI_WITH_METAL)
    add_compile_definitions(TI_WITH_METAL)
endif()

if (TI_WITH_OPENGL)
    add_compile_definitions(TI_WITH_OPENGL)
endif()

if (TI_WITH_DX11)
    add_compile_definitions(TI_WITH_DX11)
endif()

if (TI_WITH_VULKAN)
    add_compile_definitions(TI_WITH_VULKAN)
endif ()
if (TI_WITH_AMDGPU)
    add_compile_definitions(TI_WITH_AMDGPU)
endif ()
## llvm
if(TI_WITH_LLVM)
    if(DEFINED ENV{LLVM_DIR})
        set(LLVM_DIR $ENV{LLVM_DIR})
        message("Getting LLVM_DIR=${LLVM_DIR} from the environment variable")
    endif()

    # http://llvm.org/docs/CMake.html#embedding-llvm-in-your-project
    find_package(LLVM REQUIRED CONFIG)
    message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
    if(${LLVM_PACKAGE_VERSION} VERSION_LESS "10.0")
        message(FATAL_ERROR "LLVM version < 10 is not supported")
    endif()
    message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

    message("LLVM include dirs ${LLVM_INCLUDE_DIRS}")
    message("LLVM library dirs ${LLVM_LIBRARY_DIRS}")
    message("LLVM_DEFINITIONS: ${LLVM_DEFINITIONS}")
    add_definitions(${LLVM_DEFINITIONS})

    llvm_map_components_to_libnames(llvm_libs
            Core
            ExecutionEngine
            InstCombine
            OrcJIT
            RuntimeDyld
            TransformUtils
            BitReader
            BitWriter
            Object
            ScalarOpts
            Support
            native
            Linker
            Target
            MC
            Passes
            ipo
            Analysis
            )
endif(TI_WITH_LLVM)


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

## test
file(GLOB SRC_TESTS
    tests/cpp/common/*.cpp
    tests/cpp/struct/*.cpp
    tests/cpp/task/*.cpp
    tests/cpp/analysis/*.cpp
    tests/cpp/ir/*.cpp
    tests/cpp/program/*.cpp
    tests/cpp/llvm/*.cpp # have bug ?
    )
set(SRC_TESTS ${SRC_TESTS} main.cpp)
add_executable(study_vulkan
    ${SRC_TESTS}
    )
target_link_libraries(study_vulkan PRIVATE
    gtest_main
    taichi_core
    spirv_codegen
    taichi_ui
    ${llvm_libs}
    ${Eigen_LIBS}
)
if (LINUX)
    target_link_libraries(study_vulkan PRIVATE /usr/lib/x86_64-linux-gnu/libX11.so)
endif(LINUX)
