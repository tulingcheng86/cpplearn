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
CMAKE_SOURCE_DIR = /home/sbim/cpplearn/webserver/12_select

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/sbim/cpplearn/webserver/12_select/build

# Include any dependencies generated for this target.
include CMakeFiles/select_client.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/select_client.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/select_client.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/select_client.dir/flags.make

CMakeFiles/select_client.dir/select_client.cpp.o: CMakeFiles/select_client.dir/flags.make
CMakeFiles/select_client.dir/select_client.cpp.o: /home/sbim/cpplearn/webserver/12_select/select_client.cpp
CMakeFiles/select_client.dir/select_client.cpp.o: CMakeFiles/select_client.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/sbim/cpplearn/webserver/12_select/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/select_client.dir/select_client.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/select_client.dir/select_client.cpp.o -MF CMakeFiles/select_client.dir/select_client.cpp.o.d -o CMakeFiles/select_client.dir/select_client.cpp.o -c /home/sbim/cpplearn/webserver/12_select/select_client.cpp

CMakeFiles/select_client.dir/select_client.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/select_client.dir/select_client.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/sbim/cpplearn/webserver/12_select/select_client.cpp > CMakeFiles/select_client.dir/select_client.cpp.i

CMakeFiles/select_client.dir/select_client.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/select_client.dir/select_client.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/sbim/cpplearn/webserver/12_select/select_client.cpp -o CMakeFiles/select_client.dir/select_client.cpp.s

# Object files for target select_client
select_client_OBJECTS = \
"CMakeFiles/select_client.dir/select_client.cpp.o"

# External object files for target select_client
select_client_EXTERNAL_OBJECTS =

select_client: CMakeFiles/select_client.dir/select_client.cpp.o
select_client: CMakeFiles/select_client.dir/build.make
select_client: CMakeFiles/select_client.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/sbim/cpplearn/webserver/12_select/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable select_client"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/select_client.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/select_client.dir/build: select_client
.PHONY : CMakeFiles/select_client.dir/build

CMakeFiles/select_client.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/select_client.dir/cmake_clean.cmake
.PHONY : CMakeFiles/select_client.dir/clean

CMakeFiles/select_client.dir/depend:
	cd /home/sbim/cpplearn/webserver/12_select/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/sbim/cpplearn/webserver/12_select /home/sbim/cpplearn/webserver/12_select /home/sbim/cpplearn/webserver/12_select/build /home/sbim/cpplearn/webserver/12_select/build /home/sbim/cpplearn/webserver/12_select/build/CMakeFiles/select_client.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/select_client.dir/depend
