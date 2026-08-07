
add_subdirectory(common)
add_subdirectory(util)
add_subdirectory(compilation_manager)
add_subdirectory(rhi)
add_subdirectory(ui)

##
#add_subdirectory(codegen/cpu)
#add_subdirectory(codegen/spirv)
## GLOB, GLOB_RECURSE
file(GLOB TAIHI_CORE_SOURCE
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

## This version var is only used to locate slim_libdevice.10.bc
if(NOT CUDA_VERSION)
    set(CUDA_VERSION 10.0)
endif()
if (TI_WITH_CUDA)
  file(GLOB TAICHI_CUDA_RUNTIME_SOURCE "runtime/cuda/runtime.cpp")
  list(APPEND TAICHI_CORE_SOURCE ${TAICHI_CUDA_RUNTIME_SOURCE})
endif()
if (TI_WITH_AMDGPU)
  file(GLOB TAICHI_AMDGPU_RUNTIME_SOURCE "runtime/amdgpu/runtime.cpp")
  list(APPEND TAIHI_CORE_SOURCE ${TAICHI_AMDGPU_RUNTIME_SOURCE})
endif()

###
set(CORE_LIBRARY_NAME taichi_core)
add_library(${CORE_LIBRARY_NAME} SHARED
    ${TAIHI_CORE_SOURCE}
    )
target_link_libraries(${CORE_LIBRARY_NAME} PRIVATE
    ${Eigen_LIBS}
    ti_device_api
    taichi_util
    taichi_common
    compilation_manager
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

#    message("LLVM include dirs ${LLVM_INCLUDE_DIRS}")
#    message("LLVM library dirs ${LLVM_LIBRARY_DIRS}")
#    message("LLVM_DEFINITIONS: ${LLVM_DEFINITIONS}")
#    add_definitions(${LLVM_DEFINITIONS})

#    llvm_map_components_to_libnames(llvm_libs
#            Core
#            ExecutionEngine
#            InstCombine
#            OrcJIT
#            RuntimeDyld
#            TransformUtils
#            BitReader
#            BitWriter
#            Object
#            ScalarOpts
#            Support
#            native
#            Linker
#            Target
#            MC
#            Passes
#            ipo
#            Analysis
#            )

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
##
if (TI_WITH_METAL OR TI_WITH_OPENGL OR TI_WITH_DX11 OR TI_WITH_VULKAN)
    add_subdirectory(runtime/program_impls/gfx)
    target_link_libraries(${CORE_LIBRARY_NAME} PRIVATE gfx_program_impl)
endif()

if (TI_WITH_METAL)
    add_subdirectory(runtime/program_impls/metal)
    target_link_libraries(${CORE_LIBRARY_NAME} PRIVATE metal_program_impl)
endif()

if (TI_WITH_OPENGL)
    add_subdirectory(runtime/program_impls/opengl)
    target_link_libraries(${CORE_LIBRARY_NAME} PRIVATE opengl_program_impl)
endif()

if (TI_WITH_DX11)
    add_subdirectory(runtime/program_impls/dx)
    target_link_libraries(${CORE_LIBRARY_NAME} PRIVATE dx_program_impl)
endif()

if (TI_WITH_VULKAN)
    add_subdirectory(runtime/program_impls/vulkan)
    target_link_libraries(${CORE_LIBRARY_NAME} PRIVATE vulkan_program_impl)
endif ()


# SPIR-V codegen is always there, regardless of Vulkan
add_subdirectory(codegen/spirv)
add_subdirectory(runtime/gfx)

if (TI_WITH_OPENGL OR TI_WITH_VULKAN OR TI_WITH_DX11 OR TI_WITH_METAL)
  target_link_libraries(${CORE_LIBRARY_NAME} PRIVATE spirv_codegen)
  target_link_libraries(${CORE_LIBRARY_NAME} PRIVATE gfx_runtime)
endif()

#### left

# Optional dependencies
if (APPLE)
    set(APPLE_FRAMEWORKS "")
    find_library(Foundation NAMES Foundation REQUIRED)
    find_library(Metal NAMES Metal REQUIRED)
    list(APPEND APPLE_FRAMEWORKS ${Foundation} ${Metal})
    if (NOT IOS)
        find_library(ApplicationServices NAMES ApplicationServices REQUIRED)
        find_library(Cocoa NAMES Cocoa REQUIRED)
        list(APPEND APPLE_FRAMEWORKS ${ApplicationServices} ${Cocoa})
    endif()
    target_link_libraries(${CORE_LIBRARY_NAME} PRIVATE ${APPLE_FRAMEWORKS})
endif ()

if (ANDROID)
    # Android has a custom toolchain so pthread is not available and should
    # link against other libraries as well for logcat and internal features.
    target_link_libraries(${CORE_LIBRARY_NAME} PRIVATE android log)
elseif (LINUX)
    target_link_libraries(${CORE_LIBRARY_NAME} PRIVATE X11 pthread)
    if (${CMAKE_HOST_SYSTEM_PROCESSOR} STREQUAL "x86_64")
        # Avoid glibc dependencies
        if (TI_WITH_VULKAN)
            target_link_options(${CORE_LIBRARY_NAME} PRIVATE -Wl,--wrap=log2f)
        else()
            # Enforce compatibility with manylinux2014
            target_link_options(${CORE_LIBRARY_NAME} PRIVATE -Wl,--wrap=log2f -Wl,--wrap=exp2 -Wl,--wrap=log2 -Wl,--wrap=logf -Wl,--wrap=powf -Wl,--wrap=exp -Wl,--wrap=log -Wl,--wrap=pow)
        endif()
    endif()
elseif (WIN32)
    target_link_libraries(${CORE_LIBRARY_NAME} PRIVATE Winmm)
endif()

#foreach (source IN LISTS TAICHI_CORE_SOURCE)
#    file(RELATIVE_PATH source_rel ${CMAKE_CURRENT_LIST_DIR} ${source})
#    get_filename_component(source_path "${source_rel}" PATH)
#    string(REPLACE "/" "\\" source_path_msvc "${source_path}")
#    source_group("${source_path_msvc}" FILES "${source}")
#endforeach ()

if (NOT APPLE)
    install(FILES ${CMAKE_SOURCE_DIR}/external/cuda_libdevice/slim_libdevice.10.bc
            DESTINATION ${INSTALL_LIB_DIR}/runtime)
endif()

if (TI_WITH_AMDGPU)
    file(GLOB AMDGPU_BC_FILES ${CMAKE_SOURCE_DIR}/external/amdgpu_libdevice/*.bc)
    install(FILES ${AMDGPU_BC_FILES}
            DESTINATION ${INSTALL_LIB_DIR}/runtime)
endif()
