cmake_minimum_required (VERSION 3.8)
project (os_lab)

add_subdirectory(shared)
add_subdirectory(nl)
add_subdirectory(head)
add_subdirectory(chroot)
add_subdirectory(mush)

add_subdirectory(standalone/nl)
add_subdirectory(standalone/head)
add_subdirectory(standalone/chroot)
