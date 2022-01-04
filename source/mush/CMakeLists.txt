cmake_minimum_required (VERSION 3.8)
project (os_lab_mush)

set(_src_root_path "${CMAKE_CURRENT_SOURCE_DIR}")

file(GLOB_RECURSE _files LIST_DIRECTORIES false "${_src_root_path}/*.cpp" "${_src_root_path}/*.h" "${_src_root_path}/*.hpp" )

add_executable(os_lab_mush ${_files})

add_custom_command(TARGET os_lab_mush 
                   POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:os_lab_mush> ${CMAKE_CURRENT_SOURCE_DIR}/../../final/mush)

target_link_libraries(os_lab_mush PUBLIC os_lab_shared)
target_link_libraries(os_lab_mush PUBLIC os_lab_nl)
target_link_libraries(os_lab_mush PUBLIC os_lab_head)
target_link_libraries(os_lab_mush PUBLIC os_lab_chroot)
