# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

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

# The program to use to edit the cache.
CMAKE_EDIT_COMMAND = /usr/bin/ccmake

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "/homes/rb812/Dropbox/Imperial College/Advanced-Graphics"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/homes/rb812/Dropbox/Imperial College/Advanced-Graphics/build"

# Include any dependencies generated for this target.
include CMakeFiles/HDRimage.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/HDRimage.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/HDRimage.dir/flags.make

CMakeFiles/HDRimage.dir/src/main.cpp.o: CMakeFiles/HDRimage.dir/flags.make
CMakeFiles/HDRimage.dir/src/main.cpp.o: ../src/main.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report "/homes/rb812/Dropbox/Imperial College/Advanced-Graphics/build/CMakeFiles" $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/HDRimage.dir/src/main.cpp.o"
	/usr/bin/g++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/HDRimage.dir/src/main.cpp.o -c "/homes/rb812/Dropbox/Imperial College/Advanced-Graphics/src/main.cpp"

CMakeFiles/HDRimage.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/HDRimage.dir/src/main.cpp.i"
	/usr/bin/g++  $(CXX_DEFINES) $(CXX_FLAGS) -E "/homes/rb812/Dropbox/Imperial College/Advanced-Graphics/src/main.cpp" > CMakeFiles/HDRimage.dir/src/main.cpp.i

CMakeFiles/HDRimage.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/HDRimage.dir/src/main.cpp.s"
	/usr/bin/g++  $(CXX_DEFINES) $(CXX_FLAGS) -S "/homes/rb812/Dropbox/Imperial College/Advanced-Graphics/src/main.cpp" -o CMakeFiles/HDRimage.dir/src/main.cpp.s

CMakeFiles/HDRimage.dir/src/main.cpp.o.requires:
.PHONY : CMakeFiles/HDRimage.dir/src/main.cpp.o.requires

CMakeFiles/HDRimage.dir/src/main.cpp.o.provides: CMakeFiles/HDRimage.dir/src/main.cpp.o.requires
	$(MAKE) -f CMakeFiles/HDRimage.dir/build.make CMakeFiles/HDRimage.dir/src/main.cpp.o.provides.build
.PHONY : CMakeFiles/HDRimage.dir/src/main.cpp.o.provides

CMakeFiles/HDRimage.dir/src/main.cpp.o.provides.build: CMakeFiles/HDRimage.dir/src/main.cpp.o

CMakeFiles/HDRimage.dir/src/HDRimage.cpp.o: CMakeFiles/HDRimage.dir/flags.make
CMakeFiles/HDRimage.dir/src/HDRimage.cpp.o: ../src/HDRimage.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report "/homes/rb812/Dropbox/Imperial College/Advanced-Graphics/build/CMakeFiles" $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/HDRimage.dir/src/HDRimage.cpp.o"
	/usr/bin/g++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/HDRimage.dir/src/HDRimage.cpp.o -c "/homes/rb812/Dropbox/Imperial College/Advanced-Graphics/src/HDRimage.cpp"

CMakeFiles/HDRimage.dir/src/HDRimage.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/HDRimage.dir/src/HDRimage.cpp.i"
	/usr/bin/g++  $(CXX_DEFINES) $(CXX_FLAGS) -E "/homes/rb812/Dropbox/Imperial College/Advanced-Graphics/src/HDRimage.cpp" > CMakeFiles/HDRimage.dir/src/HDRimage.cpp.i

CMakeFiles/HDRimage.dir/src/HDRimage.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/HDRimage.dir/src/HDRimage.cpp.s"
	/usr/bin/g++  $(CXX_DEFINES) $(CXX_FLAGS) -S "/homes/rb812/Dropbox/Imperial College/Advanced-Graphics/src/HDRimage.cpp" -o CMakeFiles/HDRimage.dir/src/HDRimage.cpp.s

CMakeFiles/HDRimage.dir/src/HDRimage.cpp.o.requires:
.PHONY : CMakeFiles/HDRimage.dir/src/HDRimage.cpp.o.requires

CMakeFiles/HDRimage.dir/src/HDRimage.cpp.o.provides: CMakeFiles/HDRimage.dir/src/HDRimage.cpp.o.requires
	$(MAKE) -f CMakeFiles/HDRimage.dir/build.make CMakeFiles/HDRimage.dir/src/HDRimage.cpp.o.provides.build
.PHONY : CMakeFiles/HDRimage.dir/src/HDRimage.cpp.o.provides

CMakeFiles/HDRimage.dir/src/HDRimage.cpp.o.provides.build: CMakeFiles/HDRimage.dir/src/HDRimage.cpp.o

# Object files for target HDRimage
HDRimage_OBJECTS = \
"CMakeFiles/HDRimage.dir/src/main.cpp.o" \
"CMakeFiles/HDRimage.dir/src/HDRimage.cpp.o"

# External object files for target HDRimage
HDRimage_EXTERNAL_OBJECTS =

HDRimage: CMakeFiles/HDRimage.dir/src/main.cpp.o
HDRimage: CMakeFiles/HDRimage.dir/src/HDRimage.cpp.o
HDRimage: CMakeFiles/HDRimage.dir/build.make
HDRimage: CMakeFiles/HDRimage.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable HDRimage"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/HDRimage.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/HDRimage.dir/build: HDRimage
.PHONY : CMakeFiles/HDRimage.dir/build

CMakeFiles/HDRimage.dir/requires: CMakeFiles/HDRimage.dir/src/main.cpp.o.requires
CMakeFiles/HDRimage.dir/requires: CMakeFiles/HDRimage.dir/src/HDRimage.cpp.o.requires
.PHONY : CMakeFiles/HDRimage.dir/requires

CMakeFiles/HDRimage.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/HDRimage.dir/cmake_clean.cmake
.PHONY : CMakeFiles/HDRimage.dir/clean

CMakeFiles/HDRimage.dir/depend:
	cd "/homes/rb812/Dropbox/Imperial College/Advanced-Graphics/build" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/homes/rb812/Dropbox/Imperial College/Advanced-Graphics" "/homes/rb812/Dropbox/Imperial College/Advanced-Graphics" "/homes/rb812/Dropbox/Imperial College/Advanced-Graphics/build" "/homes/rb812/Dropbox/Imperial College/Advanced-Graphics/build" "/homes/rb812/Dropbox/Imperial College/Advanced-Graphics/build/CMakeFiles/HDRimage.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : CMakeFiles/HDRimage.dir/depend

