# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.15

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
CMAKE_COMMAND = /cygdrive/c/Users/Admin/.CLion2019.3/system/cygwin_cmake/bin/cmake.exe

# The command to remove a file.
RM = /cygdrive/c/Users/Admin/.CLion2019.3/system/cygwin_cmake/bin/cmake.exe -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /cygdrive/c/Projects/thread_web_server

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /cygdrive/c/Projects/thread_web_server/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/thread_web_server.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/thread_web_server.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/thread_web_server.dir/flags.make

CMakeFiles/thread_web_server.dir/OpenMP_1.cpp.o: CMakeFiles/thread_web_server.dir/flags.make
CMakeFiles/thread_web_server.dir/OpenMP_1.cpp.o: ../OpenMP_1.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/cygdrive/c/Projects/thread_web_server/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/thread_web_server.dir/OpenMP_1.cpp.o"
	/usr/bin/c++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/thread_web_server.dir/OpenMP_1.cpp.o -c /cygdrive/c/Projects/thread_web_server/OpenMP_1.cpp

CMakeFiles/thread_web_server.dir/OpenMP_1.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/thread_web_server.dir/OpenMP_1.cpp.i"
	/usr/bin/c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /cygdrive/c/Projects/thread_web_server/OpenMP_1.cpp > CMakeFiles/thread_web_server.dir/OpenMP_1.cpp.i

CMakeFiles/thread_web_server.dir/OpenMP_1.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/thread_web_server.dir/OpenMP_1.cpp.s"
	/usr/bin/c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /cygdrive/c/Projects/thread_web_server/OpenMP_1.cpp -o CMakeFiles/thread_web_server.dir/OpenMP_1.cpp.s

# Object files for target thread_web_server
thread_web_server_OBJECTS = \
"CMakeFiles/thread_web_server.dir/OpenMP_1.cpp.o"

# External object files for target thread_web_server
thread_web_server_EXTERNAL_OBJECTS =

thread_web_server.exe: CMakeFiles/thread_web_server.dir/OpenMP_1.cpp.o
thread_web_server.exe: CMakeFiles/thread_web_server.dir/build.make
thread_web_server.exe: /usr/lib/gcc/x86_64-pc-cygwin/9.3.0/libgomp.dll.a
thread_web_server.exe: CMakeFiles/thread_web_server.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/cygdrive/c/Projects/thread_web_server/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable thread_web_server.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/thread_web_server.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/thread_web_server.dir/build: thread_web_server.exe

.PHONY : CMakeFiles/thread_web_server.dir/build

CMakeFiles/thread_web_server.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/thread_web_server.dir/cmake_clean.cmake
.PHONY : CMakeFiles/thread_web_server.dir/clean

CMakeFiles/thread_web_server.dir/depend:
	cd /cygdrive/c/Projects/thread_web_server/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /cygdrive/c/Projects/thread_web_server /cygdrive/c/Projects/thread_web_server /cygdrive/c/Projects/thread_web_server/cmake-build-debug /cygdrive/c/Projects/thread_web_server/cmake-build-debug /cygdrive/c/Projects/thread_web_server/cmake-build-debug/CMakeFiles/thread_web_server.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/thread_web_server.dir/depend
