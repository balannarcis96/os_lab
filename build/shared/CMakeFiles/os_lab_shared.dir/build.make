# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/narcis/dev/os_lab/source

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/narcis/dev/os_lab/build

# Include any dependencies generated for this target.
include shared/CMakeFiles/os_lab_shared.dir/depend.make

# Include the progress variables for this target.
include shared/CMakeFiles/os_lab_shared.dir/progress.make

# Include the compile flags for this target's objects.
include shared/CMakeFiles/os_lab_shared.dir/flags.make

shared/CMakeFiles/os_lab_shared.dir/command.cpp.o: shared/CMakeFiles/os_lab_shared.dir/flags.make
shared/CMakeFiles/os_lab_shared.dir/command.cpp.o: /home/narcis/dev/os_lab/source/shared/command.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/narcis/dev/os_lab/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object shared/CMakeFiles/os_lab_shared.dir/command.cpp.o"
	cd /home/narcis/dev/os_lab/build/shared && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/os_lab_shared.dir/command.cpp.o -c /home/narcis/dev/os_lab/source/shared/command.cpp

shared/CMakeFiles/os_lab_shared.dir/command.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/os_lab_shared.dir/command.cpp.i"
	cd /home/narcis/dev/os_lab/build/shared && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/narcis/dev/os_lab/source/shared/command.cpp > CMakeFiles/os_lab_shared.dir/command.cpp.i

shared/CMakeFiles/os_lab_shared.dir/command.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/os_lab_shared.dir/command.cpp.s"
	cd /home/narcis/dev/os_lab/build/shared && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/narcis/dev/os_lab/source/shared/command.cpp -o CMakeFiles/os_lab_shared.dir/command.cpp.s

shared/CMakeFiles/os_lab_shared.dir/shared.cpp.o: shared/CMakeFiles/os_lab_shared.dir/flags.make
shared/CMakeFiles/os_lab_shared.dir/shared.cpp.o: /home/narcis/dev/os_lab/source/shared/shared.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/narcis/dev/os_lab/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object shared/CMakeFiles/os_lab_shared.dir/shared.cpp.o"
	cd /home/narcis/dev/os_lab/build/shared && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/os_lab_shared.dir/shared.cpp.o -c /home/narcis/dev/os_lab/source/shared/shared.cpp

shared/CMakeFiles/os_lab_shared.dir/shared.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/os_lab_shared.dir/shared.cpp.i"
	cd /home/narcis/dev/os_lab/build/shared && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/narcis/dev/os_lab/source/shared/shared.cpp > CMakeFiles/os_lab_shared.dir/shared.cpp.i

shared/CMakeFiles/os_lab_shared.dir/shared.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/os_lab_shared.dir/shared.cpp.s"
	cd /home/narcis/dev/os_lab/build/shared && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/narcis/dev/os_lab/source/shared/shared.cpp -o CMakeFiles/os_lab_shared.dir/shared.cpp.s

shared/CMakeFiles/os_lab_shared.dir/utils.cpp.o: shared/CMakeFiles/os_lab_shared.dir/flags.make
shared/CMakeFiles/os_lab_shared.dir/utils.cpp.o: /home/narcis/dev/os_lab/source/shared/utils.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/narcis/dev/os_lab/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object shared/CMakeFiles/os_lab_shared.dir/utils.cpp.o"
	cd /home/narcis/dev/os_lab/build/shared && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/os_lab_shared.dir/utils.cpp.o -c /home/narcis/dev/os_lab/source/shared/utils.cpp

shared/CMakeFiles/os_lab_shared.dir/utils.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/os_lab_shared.dir/utils.cpp.i"
	cd /home/narcis/dev/os_lab/build/shared && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/narcis/dev/os_lab/source/shared/utils.cpp > CMakeFiles/os_lab_shared.dir/utils.cpp.i

shared/CMakeFiles/os_lab_shared.dir/utils.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/os_lab_shared.dir/utils.cpp.s"
	cd /home/narcis/dev/os_lab/build/shared && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/narcis/dev/os_lab/source/shared/utils.cpp -o CMakeFiles/os_lab_shared.dir/utils.cpp.s

# Object files for target os_lab_shared
os_lab_shared_OBJECTS = \
"CMakeFiles/os_lab_shared.dir/command.cpp.o" \
"CMakeFiles/os_lab_shared.dir/shared.cpp.o" \
"CMakeFiles/os_lab_shared.dir/utils.cpp.o"

# External object files for target os_lab_shared
os_lab_shared_EXTERNAL_OBJECTS =

shared/libos_lab_shared.a: shared/CMakeFiles/os_lab_shared.dir/command.cpp.o
shared/libos_lab_shared.a: shared/CMakeFiles/os_lab_shared.dir/shared.cpp.o
shared/libos_lab_shared.a: shared/CMakeFiles/os_lab_shared.dir/utils.cpp.o
shared/libos_lab_shared.a: shared/CMakeFiles/os_lab_shared.dir/build.make
shared/libos_lab_shared.a: shared/CMakeFiles/os_lab_shared.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/narcis/dev/os_lab/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX static library libos_lab_shared.a"
	cd /home/narcis/dev/os_lab/build/shared && $(CMAKE_COMMAND) -P CMakeFiles/os_lab_shared.dir/cmake_clean_target.cmake
	cd /home/narcis/dev/os_lab/build/shared && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/os_lab_shared.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
shared/CMakeFiles/os_lab_shared.dir/build: shared/libos_lab_shared.a

.PHONY : shared/CMakeFiles/os_lab_shared.dir/build

shared/CMakeFiles/os_lab_shared.dir/clean:
	cd /home/narcis/dev/os_lab/build/shared && $(CMAKE_COMMAND) -P CMakeFiles/os_lab_shared.dir/cmake_clean.cmake
.PHONY : shared/CMakeFiles/os_lab_shared.dir/clean

shared/CMakeFiles/os_lab_shared.dir/depend:
	cd /home/narcis/dev/os_lab/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/narcis/dev/os_lab/source /home/narcis/dev/os_lab/source/shared /home/narcis/dev/os_lab/build /home/narcis/dev/os_lab/build/shared /home/narcis/dev/os_lab/build/shared/CMakeFiles/os_lab_shared.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : shared/CMakeFiles/os_lab_shared.dir/depend
