# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.27

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /home/sbim/.local/lib/python3.10/site-packages/cmake/data/bin/cmake

# The command to remove a file.
RM = /home/sbim/.local/lib/python3.10/site-packages/cmake/data/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "/home/sbim/cpplearn/webserver/14 multicast and broadcast"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/home/sbim/cpplearn/webserver/14 multicast and broadcast/build"

# Include any dependencies generated for this target.
include CMakeFiles/2_receiver.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/2_receiver.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/2_receiver.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/2_receiver.dir/flags.make

CMakeFiles/2_receiver.dir/2_receiver.cpp.o: CMakeFiles/2_receiver.dir/flags.make
CMakeFiles/2_receiver.dir/2_receiver.cpp.o: /home/sbim/cpplearn/webserver/14\ multicast\ and\ broadcast/2_receiver.cpp
CMakeFiles/2_receiver.dir/2_receiver.cpp.o: CMakeFiles/2_receiver.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir="/home/sbim/cpplearn/webserver/14 multicast and broadcast/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/2_receiver.dir/2_receiver.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/2_receiver.dir/2_receiver.cpp.o -MF CMakeFiles/2_receiver.dir/2_receiver.cpp.o.d -o CMakeFiles/2_receiver.dir/2_receiver.cpp.o -c "/home/sbim/cpplearn/webserver/14 multicast and broadcast/2_receiver.cpp"

CMakeFiles/2_receiver.dir/2_receiver.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/2_receiver.dir/2_receiver.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/home/sbim/cpplearn/webserver/14 multicast and broadcast/2_receiver.cpp" > CMakeFiles/2_receiver.dir/2_receiver.cpp.i

CMakeFiles/2_receiver.dir/2_receiver.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/2_receiver.dir/2_receiver.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/home/sbim/cpplearn/webserver/14 multicast and broadcast/2_receiver.cpp" -o CMakeFiles/2_receiver.dir/2_receiver.cpp.s

# Object files for target 2_receiver
2_receiver_OBJECTS = \
"CMakeFiles/2_receiver.dir/2_receiver.cpp.o"

# External object files for target 2_receiver
2_receiver_EXTERNAL_OBJECTS =

2_receiver: CMakeFiles/2_receiver.dir/2_receiver.cpp.o
2_receiver: CMakeFiles/2_receiver.dir/build.make
2_receiver: CMakeFiles/2_receiver.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir="/home/sbim/cpplearn/webserver/14 multicast and broadcast/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable 2_receiver"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/2_receiver.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/2_receiver.dir/build: 2_receiver
.PHONY : CMakeFiles/2_receiver.dir/build

CMakeFiles/2_receiver.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/2_receiver.dir/cmake_clean.cmake
.PHONY : CMakeFiles/2_receiver.dir/clean

CMakeFiles/2_receiver.dir/depend:
	cd "/home/sbim/cpplearn/webserver/14 multicast and broadcast/build" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/home/sbim/cpplearn/webserver/14 multicast and broadcast" "/home/sbim/cpplearn/webserver/14 multicast and broadcast" "/home/sbim/cpplearn/webserver/14 multicast and broadcast/build" "/home/sbim/cpplearn/webserver/14 multicast and broadcast/build" "/home/sbim/cpplearn/webserver/14 multicast and broadcast/build/CMakeFiles/2_receiver.dir/DependInfo.cmake" "--color=$(COLOR)"
.PHONY : CMakeFiles/2_receiver.dir/depend

