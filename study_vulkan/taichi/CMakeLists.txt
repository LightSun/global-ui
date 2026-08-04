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

endif(TI_WITH_LLVM)

add_subdirectory(common)
add_subdirectory(util)
add_subdirectory(rhi)
add_subdirectory(ui)

##
#add_subdirectory(codegen/cpu)
#add_subdirectory(codegen/spirv)

## GLOB, GLOB_RECURSE
file(GLOB SRC_TAICHI_CORE
    analysis/*.cpp
    analysis/*.c
    aot/*.cpp
    aot/*.c
    ir/*.cpp
    ir/*.c
    program/*.cpp
    program/*.c
    jit/*.cpp

    struct/*.cpp
    struct/*.c
    system/*.cpp
    system/*.c
    transforms/*.cpp
    "platform/cuda/*" "platform/amdgpu/*"
    "platform/mac/*" "platform/windows/*"
    "codegen/*.cpp" "codegen/*.h"
    "runtime/*.h" "runtime/*.cpp"
    )

set(CORE_LIBRARY_NAME taichi_core)
add_library(${CORE_LIBRARY_NAME} SHARED
    ${SRC_TAICHI_CORE}
    )
target_link_libraries(${CORE_LIBRARY_NAME} PRIVATE
    ${Eigen_LIBS}
    ti_device_api
)
###
###
if(TI_WITH_LLVM)
#    if(DEFINED ENV{LLVM_DIR})
#        set(LLVM_DIR $ENV{LLVM_DIR})
#        message("Getting LLVM_DIR=${LLVM_DIR} from the environment variable")
#    endif()

#    # http://llvm.org/docs/CMake.html#embedding-llvm-in-your-project
#    find_package(LLVM REQUIRED CONFIG)
#    message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
#    if(${LLVM_PACKAGE_VERSION} VERSION_LESS "10.0")
#        message(FATAL_ERROR "LLVM version < 10 is not supported")
#    endif()
#    message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")
    target_include_directories(${CORE_LIBRARY_NAME} PUBLIC ${LLVM_INCLUDE_DIRS})

    message("LLVM include dirs ${LLVM_INCLUDE_DIRS}")
    message("LLVM library dirs ${LLVM_LIBRARY_DIRS}")
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

    if (APPLE AND "${CMAKE_SYSTEM_PROCESSOR}" STREQUAL "arm64")
        llvm_map_components_to_libnames(llvm_aarch64_libs AArch64)
    endif()

    add_subdirectory(codegen/cpu)
    add_subdirectory(runtime/cpu)

    target_link_libraries(${CORE_LIBRARY_NAME} PRIVATE cpu_codegen)
    target_link_libraries(${CORE_LIBRARY_NAME} PRIVATE cpu_runtime)

    if (TI_WITH_CUDA)
        llvm_map_components_to_libnames(llvm_ptx_libs NVPTX)
        add_subdirectory(codegen/cuda)
        add_subdirectory(runtime/cuda)

        target_link_libraries(${CORE_LIBRARY_NAME} PRIVATE cuda_codegen)
        target_link_libraries(${CORE_LIBRARY_NAME} PRIVATE cuda_runtime)
    endif()

    if (TI_WITH_AMDGPU)
        llvm_map_components_to_libnames(llvm_amdgpu_libs AMDGPU)
        add_subdirectory(codegen/amdgpu)
        add_subdirectory(runtime/amdgpu)

        target_link_libraries(${CORE_LIBRARY_NAME} PRIVATE amdgpu_codegen)
        target_link_libraries(${CORE_LIBRARY_NAME} PRIVATE amdgpu_runtime)
    endif()

    if (TI_WITH_DX12)
        llvm_map_components_to_libnames(llvm_directx_libs DirectX)

        add_subdirectory(runtime/dx12)
        add_subdirectory(codegen/dx12)
        add_subdirectory(runtime/program_impls/dx12)

        target_include_directories(${CORE_LIBRARY_NAME} PRIVATE external/DirectX-Headers/include)
        target_link_libraries(${CORE_LIBRARY_NAME} PRIVATE dx12_codegen)
        target_link_libraries(${CORE_LIBRARY_NAME} PRIVATE dx12_runtime)
        target_link_libraries(${CORE_LIBRARY_NAME} PRIVATE dx12_program_impl)
    endif()

    add_subdirectory(codegen/llvm)
    add_subdirectory(runtime/llvm)
    add_subdirectory(runtime/program_impls/llvm)

    target_link_libraries(${CORE_LIBRARY_NAME} PRIVATE llvm_program_impl)
    target_link_libraries(${CORE_LIBRARY_NAME} PRIVATE llvm_codegen)
    target_link_libraries(${CORE_LIBRARY_NAME} PRIVATE llvm_runtime)

    if (LINUX)
        # Remove symbols from llvm static libs
        foreach(LETTER ${llvm_libs})
            target_link_options(${CORE_LIBRARY_NAME} PUBLIC -Wl,--exclude-libs=lib${LETTER}.a)
        endforeach()
    endif()
endif()
