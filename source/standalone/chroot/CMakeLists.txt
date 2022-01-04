cmake_minimum_required (VERSION 3.8)
project (os_lab_chroot_s)

set(_src_root_path "${CMAKE_CURRENT_SOURCE_DIR}")

file(GLOB_RECURSE _files LIST_DIRECTORIES false "${_src_root_path}/*.cpp" "${_src_root_path}/*.h" "${_src_root_path}/*.hpp" )

add_executable(os_lab_chroot_s ${_files})

add_custom_command(TARGET os_lab_chroot_s 
                   POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:os_lab_chroot_s> ${CMAKE_CURRENT_SOURCE_DIR}/../../../final/commands/chroot)

target_link_libraries(os_lab_chroot_s PUBLIC os_lab_chroot)
