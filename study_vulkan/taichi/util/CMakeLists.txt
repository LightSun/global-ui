# ./taichi/util/CMakeLists.txt

add_library(taichi_util)
target_sources(taichi_util
  PRIVATE
    bit.cpp
    file_sequence_writer.cpp
    image_buffer.cpp
    image_io.cpp
    lang_util.cpp
    offline_cache.cpp
    short_name.cpp
    str.cpp
    testing.cpp
    zip.cpp
  )

target_include_directories(taichi_util
  PRIVATE
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/external/include
    ${PROJECT_SOURCE_DIR}/external/eigen
    ${LLVM_INCLUDE_DIRS}
  )

target_link_libraries(taichi_util PUBLIC taichi_common)
if(H7_SIMPLE_TEST)
    target_compile_definitions(taichi_util PRIVATE H7_SIMPLE_TEST)
endif(H7_SIMPLE_TEST)
