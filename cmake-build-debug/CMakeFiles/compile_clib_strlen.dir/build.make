# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.29

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/mellw/libs/clib

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/mellw/libs/clib/cmake-build-debug

# Utility rule file for compile_clib_strlen.

# Include any custom commands dependencies for this target.
include CMakeFiles/compile_clib_strlen.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/compile_clib_strlen.dir/progress.make

CMakeFiles/compile_clib_strlen: /home/mellw/libs/clib/obj/clib_strlen.o

/home/mellw/libs/clib/obj/clib_strlen.o: /home/mellw/libs/clib/src/clib_strlen.asm
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --blue --bold --progress-dir=/home/mellw/libs/clib/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Compiling NASM source file: /home/mellw/libs/clib/src/clib_strlen.asm to /home/mellw/libs/clib/obj/clib_strlen.o"
	nasm -f elf64 -o /home/mellw/libs/clib/obj/clib_strlen.o /home/mellw/libs/clib/src/clib_strlen.asm

compile_clib_strlen: CMakeFiles/compile_clib_strlen
compile_clib_strlen: /home/mellw/libs/clib/obj/clib_strlen.o
compile_clib_strlen: CMakeFiles/compile_clib_strlen.dir/build.make
.PHONY : compile_clib_strlen

# Rule to build all files generated by this target.
CMakeFiles/compile_clib_strlen.dir/build: compile_clib_strlen
.PHONY : CMakeFiles/compile_clib_strlen.dir/build

CMakeFiles/compile_clib_strlen.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/compile_clib_strlen.dir/cmake_clean.cmake
.PHONY : CMakeFiles/compile_clib_strlen.dir/clean

CMakeFiles/compile_clib_strlen.dir/depend:
	cd /home/mellw/libs/clib/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/mellw/libs/clib /home/mellw/libs/clib /home/mellw/libs/clib/cmake-build-debug /home/mellw/libs/clib/cmake-build-debug /home/mellw/libs/clib/cmake-build-debug/CMakeFiles/compile_clib_strlen.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/compile_clib_strlen.dir/depend

