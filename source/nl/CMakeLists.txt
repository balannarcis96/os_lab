cmake_minimum_required (VERSION 3.8)
project (os_lab_nl)

set(_src_root_path "${CMAKE_CURRENT_SOURCE_DIR}")

file(GLOB_RECURSE _files LIST_DIRECTORIES false "${_src_root_path}/*.cpp" "${_src_root_path}/*.h" "${_src_root_path}/*.hpp" )

add_library(os_lab_nl STATIC ${_files})
target_include_directories(os_lab_nl PUBLIC "${_src_root_path}/")

target_link_libraries(os_lab_nl PUBLIC os_lab_shared)
