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
CMAKE_COMMAND = /home/alhuber/clion-2019.3.3/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /home/alhuber/clion-2019.3.3/bin/cmake/linux/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "/home/alhuber/Schreibtisch/HTWSaar/Kommunikationsinformatik Master/3.Semester/VAA (1.Semester)/Uebungen/1.Uebung/repo/VAA"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/home/alhuber/Schreibtisch/HTWSaar/Kommunikationsinformatik Master/3.Semester/VAA (1.Semester)/Uebungen/1.Uebung/repo/VAA/cmake-build-debug"

# Include any dependencies generated for this target.
include CMakeFiles/graphGenApp.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/graphGenApp.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/graphGenApp.dir/flags.make

CMakeFiles/graphGenApp.dir/src/Handler/floodHandler.cpp.o: CMakeFiles/graphGenApp.dir/flags.make
CMakeFiles/graphGenApp.dir/src/Handler/floodHandler.cpp.o: ../src/Handler/floodHandler.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/home/alhuber/Schreibtisch/HTWSaar/Kommunikationsinformatik Master/3.Semester/VAA (1.Semester)/Uebungen/1.Uebung/repo/VAA/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/graphGenApp.dir/src/Handler/floodHandler.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/graphGenApp.dir/src/Handler/floodHandler.cpp.o -c "/home/alhuber/Schreibtisch/HTWSaar/Kommunikationsinformatik Master/3.Semester/VAA (1.Semester)/Uebungen/1.Uebung/repo/VAA/src/Handler/floodHandler.cpp"

CMakeFiles/graphGenApp.dir/src/Handler/floodHandler.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/graphGenApp.dir/src/Handler/floodHandler.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/home/alhuber/Schreibtisch/HTWSaar/Kommunikationsinformatik Master/3.Semester/VAA (1.Semester)/Uebungen/1.Uebung/repo/VAA/src/Handler/floodHandler.cpp" > CMakeFiles/graphGenApp.dir/src/Handler/floodHandler.cpp.i

CMakeFiles/graphGenApp.dir/src/Handler/floodHandler.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/graphGenApp.dir/src/Handler/floodHandler.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/home/alhuber/Schreibtisch/HTWSaar/Kommunikationsinformatik Master/3.Semester/VAA (1.Semester)/Uebungen/1.Uebung/repo/VAA/src/Handler/floodHandler.cpp" -o CMakeFiles/graphGenApp.dir/src/Handler/floodHandler.cpp.s

CMakeFiles/graphGenApp.dir/src/Handler/configHandler.cpp.o: CMakeFiles/graphGenApp.dir/flags.make
CMakeFiles/graphGenApp.dir/src/Handler/configHandler.cpp.o: ../src/Handler/configHandler.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/home/alhuber/Schreibtisch/HTWSaar/Kommunikationsinformatik Master/3.Semester/VAA (1.Semester)/Uebungen/1.Uebung/repo/VAA/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/graphGenApp.dir/src/Handler/configHandler.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/graphGenApp.dir/src/Handler/configHandler.cpp.o -c "/home/alhuber/Schreibtisch/HTWSaar/Kommunikationsinformatik Master/3.Semester/VAA (1.Semester)/Uebungen/1.Uebung/repo/VAA/src/Handler/configHandler.cpp"

CMakeFiles/graphGenApp.dir/src/Handler/configHandler.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/graphGenApp.dir/src/Handler/configHandler.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/home/alhuber/Schreibtisch/HTWSaar/Kommunikationsinformatik Master/3.Semester/VAA (1.Semester)/Uebungen/1.Uebung/repo/VAA/src/Handler/configHandler.cpp" > CMakeFiles/graphGenApp.dir/src/Handler/configHandler.cpp.i

CMakeFiles/graphGenApp.dir/src/Handler/configHandler.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/graphGenApp.dir/src/Handler/configHandler.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/home/alhuber/Schreibtisch/HTWSaar/Kommunikationsinformatik Master/3.Semester/VAA (1.Semester)/Uebungen/1.Uebung/repo/VAA/src/Handler/configHandler.cpp" -o CMakeFiles/graphGenApp.dir/src/Handler/configHandler.cpp.s

CMakeFiles/graphGenApp.dir/src/Graph/edge.cpp.o: CMakeFiles/graphGenApp.dir/flags.make
CMakeFiles/graphGenApp.dir/src/Graph/edge.cpp.o: ../src/Graph/edge.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/home/alhuber/Schreibtisch/HTWSaar/Kommunikationsinformatik Master/3.Semester/VAA (1.Semester)/Uebungen/1.Uebung/repo/VAA/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/graphGenApp.dir/src/Graph/edge.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/graphGenApp.dir/src/Graph/edge.cpp.o -c "/home/alhuber/Schreibtisch/HTWSaar/Kommunikationsinformatik Master/3.Semester/VAA (1.Semester)/Uebungen/1.Uebung/repo/VAA/src/Graph/edge.cpp"

CMakeFiles/graphGenApp.dir/src/Graph/edge.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/graphGenApp.dir/src/Graph/edge.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/home/alhuber/Schreibtisch/HTWSaar/Kommunikationsinformatik Master/3.Semester/VAA (1.Semester)/Uebungen/1.Uebung/repo/VAA/src/Graph/edge.cpp" > CMakeFiles/graphGenApp.dir/src/Graph/edge.cpp.i

CMakeFiles/graphGenApp.dir/src/Graph/edge.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/graphGenApp.dir/src/Graph/edge.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/home/alhuber/Schreibtisch/HTWSaar/Kommunikationsinformatik Master/3.Semester/VAA (1.Semester)/Uebungen/1.Uebung/repo/VAA/src/Graph/edge.cpp" -o CMakeFiles/graphGenApp.dir/src/Graph/edge.cpp.s

CMakeFiles/graphGenApp.dir/src/Handler/goldmanEdgeChasingHandler.cpp.o: CMakeFiles/graphGenApp.dir/flags.make
CMakeFiles/graphGenApp.dir/src/Handler/goldmanEdgeChasingHandler.cpp.o: ../src/Handler/goldmanEdgeChasingHandler.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/home/alhuber/Schreibtisch/HTWSaar/Kommunikationsinformatik Master/3.Semester/VAA (1.Semester)/Uebungen/1.Uebung/repo/VAA/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/graphGenApp.dir/src/Handler/goldmanEdgeChasingHandler.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/graphGenApp.dir/src/Handler/goldmanEdgeChasingHandler.cpp.o -c "/home/alhuber/Schreibtisch/HTWSaar/Kommunikationsinformatik Master/3.Semester/VAA (1.Semester)/Uebungen/1.Uebung/repo/VAA/src/Handler/goldmanEdgeChasingHandler.cpp"

CMakeFiles/graphGenApp.dir/src/Handler/goldmanEdgeChasingHandler.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/graphGenApp.dir/src/Handler/goldmanEdgeChasingHandler.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/home/alhuber/Schreibtisch/HTWSaar/Kommunikationsinformatik Master/3.Semester/VAA (1.Semester)/Uebungen/1.Uebung/repo/VAA/src/Handler/goldmanEdgeChasingHandler.cpp" > CMakeFiles/graphGenApp.dir/src/Handler/goldmanEdgeChasingHandler.cpp.i

CMakeFiles/graphGenApp.dir/src/Handler/goldmanEdgeChasingHandler.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/graphGenApp.dir/src/Handler/goldmanEdgeChasingHandler.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/home/alhuber/Schreibtisch/HTWSaar/Kommunikationsinformatik Master/3.Semester/VAA (1.Semester)/Uebungen/1.Uebung/repo/VAA/src/Handler/goldmanEdgeChasingHandler.cpp" -o CMakeFiles/graphGenApp.dir/src/Handler/goldmanEdgeChasingHandler.cpp.s

CMakeFiles/graphGenApp.dir/src/Application/agreement.cpp.o: CMakeFiles/graphGenApp.dir/flags.make
CMakeFiles/graphGenApp.dir/src/Application/agreement.cpp.o: ../src/Application/agreement.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/home/alhuber/Schreibtisch/HTWSaar/Kommunikationsinformatik Master/3.Semester/VAA (1.Semester)/Uebungen/1.Uebung/repo/VAA/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/graphGenApp.dir/src/Application/agreement.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/graphGenApp.dir/src/Application/agreement.cpp.o -c "/home/alhuber/Schreibtisch/HTWSaar/Kommunikationsinformatik Master/3.Semester/VAA (1.Semester)/Uebungen/1.Uebung/repo/VAA/src/Application/agreement.cpp"

CMakeFiles/graphGenApp.dir/src/Application/agreement.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/graphGenApp.dir/src/Application/agreement.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/home/alhuber/Schreibtisch/HTWSaar/Kommunikationsinformatik Master/3.Semester/VAA (1.Semester)/Uebungen/1.Uebung/repo/VAA/src/Application/agreement.cpp" > CMakeFiles/graphGenApp.dir/src/Application/agreement.cpp.i

CMakeFiles/graphGenApp.dir/src/Application/agreement.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/graphGenApp.dir/src/Application/agreement.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/home/alhuber/Schreibtisch/HTWSaar/Kommunikationsinformatik Master/3.Semester/VAA (1.Semester)/Uebungen/1.Uebung/repo/VAA/src/Application/agreement.cpp" -o CMakeFiles/graphGenApp.dir/src/Application/agreement.cpp.s

CMakeFiles/graphGenApp.dir/src/Graph/node.cpp.o: CMakeFiles/graphGenApp.dir/flags.make
CMakeFiles/graphGenApp.dir/src/Graph/node.cpp.o: ../src/Graph/node.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/home/alhuber/Schreibtisch/HTWSaar/Kommunikationsinformatik Master/3.Semester/VAA (1.Semester)/Uebungen/1.Uebung/repo/VAA/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/graphGenApp.dir/src/Graph/node.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/graphGenApp.dir/src/Graph/node.cpp.o -c "/home/alhuber/Schreibtisch/HTWSaar/Kommunikationsinformatik Master/3.Semester/VAA (1.Semester)/Uebungen/1.Uebung/repo/VAA/src/Graph/node.cpp"

CMakeFiles/graphGenApp.dir/src/Graph/node.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/graphGenApp.dir/src/Graph/node.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/home/alhuber/Schreibtisch/HTWSaar/Kommunikationsinformatik Master/3.Semester/VAA (1.Semester)/Uebungen/1.Uebung/repo/VAA/src/Graph/node.cpp" > CMakeFiles/graphGenApp.dir/src/Graph/node.cpp.i

CMakeFiles/graphGenApp.dir/src/Graph/node.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/graphGenApp.dir/src/Graph/node.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/home/alhuber/Schreibtisch/HTWSaar/Kommunikationsinformatik Master/3.Semester/VAA (1.Semester)/Uebungen/1.Uebung/repo/VAA/src/Graph/node.cpp" -o CMakeFiles/graphGenApp.dir/src/Graph/node.cpp.s

CMakeFiles/graphGenApp.dir/src/Handler/fileHandler.cpp.o: CMakeFiles/graphGenApp.dir/flags.make
CMakeFiles/graphGenApp.dir/src/Handler/fileHandler.cpp.o: ../src/Handler/fileHandler.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/home/alhuber/Schreibtisch/HTWSaar/Kommunikationsinformatik Master/3.Semester/VAA (1.Semester)/Uebungen/1.Uebung/repo/VAA/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/graphGenApp.dir/src/Handler/fileHandler.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/graphGenApp.dir/src/Handler/fileHandler.cpp.o -c "/home/alhuber/Schreibtisch/HTWSaar/Kommunikationsinformatik Master/3.Semester/VAA (1.Semester)/Uebungen/1.Uebung/repo/VAA/src/Handler/fileHandler.cpp"

CMakeFiles/graphGenApp.dir/src/Handler/fileHandler.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/graphGenApp.dir/src/Handler/fileHandler.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/home/alhuber/Schreibtisch/HTWSaar/Kommunikationsinformatik Master/3.Semester/VAA (1.Semester)/Uebungen/1.Uebung/repo/VAA/src/Handler/fileHandler.cpp" > CMakeFiles/graphGenApp.dir/src/Handler/fileHandler.cpp.i

CMakeFiles/graphGenApp.dir/src/Handler/fileHandler.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/graphGenApp.dir/src/Handler/fileHandler.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/home/alhuber/Schreibtisch/HTWSaar/Kommunikationsinformatik Master/3.Semester/VAA (1.Semester)/Uebungen/1.Uebung/repo/VAA/src/Handler/fileHandler.cpp" -o CMakeFiles/graphGenApp.dir/src/Handler/fileHandler.cpp.s

CMakeFiles/graphGenApp.dir/src/graphGenApp.cpp.o: CMakeFiles/graphGenApp.dir/flags.make
CMakeFiles/graphGenApp.dir/src/graphGenApp.cpp.o: ../src/graphGenApp.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/home/alhuber/Schreibtisch/HTWSaar/Kommunikationsinformatik Master/3.Semester/VAA (1.Semester)/Uebungen/1.Uebung/repo/VAA/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object CMakeFiles/graphGenApp.dir/src/graphGenApp.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/graphGenApp.dir/src/graphGenApp.cpp.o -c "/home/alhuber/Schreibtisch/HTWSaar/Kommunikationsinformatik Master/3.Semester/VAA (1.Semester)/Uebungen/1.Uebung/repo/VAA/src/graphGenApp.cpp"

CMakeFiles/graphGenApp.dir/src/graphGenApp.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/graphGenApp.dir/src/graphGenApp.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/home/alhuber/Schreibtisch/HTWSaar/Kommunikationsinformatik Master/3.Semester/VAA (1.Semester)/Uebungen/1.Uebung/repo/VAA/src/graphGenApp.cpp" > CMakeFiles/graphGenApp.dir/src/graphGenApp.cpp.i

CMakeFiles/graphGenApp.dir/src/graphGenApp.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/graphGenApp.dir/src/graphGenApp.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/home/alhuber/Schreibtisch/HTWSaar/Kommunikationsinformatik Master/3.Semester/VAA (1.Semester)/Uebungen/1.Uebung/repo/VAA/src/graphGenApp.cpp" -o CMakeFiles/graphGenApp.dir/src/graphGenApp.cpp.s

CMakeFiles/graphGenApp.dir/src/Handler/echoHandler.cpp.o: CMakeFiles/graphGenApp.dir/flags.make
CMakeFiles/graphGenApp.dir/src/Handler/echoHandler.cpp.o: ../src/Handler/echoHandler.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/home/alhuber/Schreibtisch/HTWSaar/Kommunikationsinformatik Master/3.Semester/VAA (1.Semester)/Uebungen/1.Uebung/repo/VAA/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object CMakeFiles/graphGenApp.dir/src/Handler/echoHandler.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/graphGenApp.dir/src/Handler/echoHandler.cpp.o -c "/home/alhuber/Schreibtisch/HTWSaar/Kommunikationsinformatik Master/3.Semester/VAA (1.Semester)/Uebungen/1.Uebung/repo/VAA/src/Handler/echoHandler.cpp"

CMakeFiles/graphGenApp.dir/src/Handler/echoHandler.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/graphGenApp.dir/src/Handler/echoHandler.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/home/alhuber/Schreibtisch/HTWSaar/Kommunikationsinformatik Master/3.Semester/VAA (1.Semester)/Uebungen/1.Uebung/repo/VAA/src/Handler/echoHandler.cpp" > CMakeFiles/graphGenApp.dir/src/Handler/echoHandler.cpp.i

CMakeFiles/graphGenApp.dir/src/Handler/echoHandler.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/graphGenApp.dir/src/Handler/echoHandler.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/home/alhuber/Schreibtisch/HTWSaar/Kommunikationsinformatik Master/3.Semester/VAA (1.Semester)/Uebungen/1.Uebung/repo/VAA/src/Handler/echoHandler.cpp" -o CMakeFiles/graphGenApp.dir/src/Handler/echoHandler.cpp.s

CMakeFiles/graphGenApp.dir/src/Handler/accountHandler.cpp.o: CMakeFiles/graphGenApp.dir/flags.make
CMakeFiles/graphGenApp.dir/src/Handler/accountHandler.cpp.o: ../src/Handler/accountHandler.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/home/alhuber/Schreibtisch/HTWSaar/Kommunikationsinformatik Master/3.Semester/VAA (1.Semester)/Uebungen/1.Uebung/repo/VAA/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_10) "Building CXX object CMakeFiles/graphGenApp.dir/src/Handler/accountHandler.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/graphGenApp.dir/src/Handler/accountHandler.cpp.o -c "/home/alhuber/Schreibtisch/HTWSaar/Kommunikationsinformatik Master/3.Semester/VAA (1.Semester)/Uebungen/1.Uebung/repo/VAA/src/Handler/accountHandler.cpp"

CMakeFiles/graphGenApp.dir/src/Handler/accountHandler.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/graphGenApp.dir/src/Handler/accountHandler.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/home/alhuber/Schreibtisch/HTWSaar/Kommunikationsinformatik Master/3.Semester/VAA (1.Semester)/Uebungen/1.Uebung/repo/VAA/src/Handler/accountHandler.cpp" > CMakeFiles/graphGenApp.dir/src/Handler/accountHandler.cpp.i

CMakeFiles/graphGenApp.dir/src/Handler/accountHandler.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/graphGenApp.dir/src/Handler/accountHandler.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/home/alhuber/Schreibtisch/HTWSaar/Kommunikationsinformatik Master/3.Semester/VAA (1.Semester)/Uebungen/1.Uebung/repo/VAA/src/Handler/accountHandler.cpp" -o CMakeFiles/graphGenApp.dir/src/Handler/accountHandler.cpp.s

CMakeFiles/graphGenApp.dir/src/Application/doubleCounting.cpp.o: CMakeFiles/graphGenApp.dir/flags.make
CMakeFiles/graphGenApp.dir/src/Application/doubleCounting.cpp.o: ../src/Application/doubleCounting.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/home/alhuber/Schreibtisch/HTWSaar/Kommunikationsinformatik Master/3.Semester/VAA (1.Semester)/Uebungen/1.Uebung/repo/VAA/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_11) "Building CXX object CMakeFiles/graphGenApp.dir/src/Application/doubleCounting.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/graphGenApp.dir/src/Application/doubleCounting.cpp.o -c "/home/alhuber/Schreibtisch/HTWSaar/Kommunikationsinformatik Master/3.Semester/VAA (1.Semester)/Uebungen/1.Uebung/repo/VAA/src/Application/doubleCounting.cpp"

CMakeFiles/graphGenApp.dir/src/Application/doubleCounting.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/graphGenApp.dir/src/Application/doubleCounting.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/home/alhuber/Schreibtisch/HTWSaar/Kommunikationsinformatik Master/3.Semester/VAA (1.Semester)/Uebungen/1.Uebung/repo/VAA/src/Application/doubleCounting.cpp" > CMakeFiles/graphGenApp.dir/src/Application/doubleCounting.cpp.i

CMakeFiles/graphGenApp.dir/src/Application/doubleCounting.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/graphGenApp.dir/src/Application/doubleCounting.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/home/alhuber/Schreibtisch/HTWSaar/Kommunikationsinformatik Master/3.Semester/VAA (1.Semester)/Uebungen/1.Uebung/repo/VAA/src/Application/doubleCounting.cpp" -o CMakeFiles/graphGenApp.dir/src/Application/doubleCounting.cpp.s

CMakeFiles/graphGenApp.dir/src/Handler/messageHandler.cpp.o: CMakeFiles/graphGenApp.dir/flags.make
CMakeFiles/graphGenApp.dir/src/Handler/messageHandler.cpp.o: ../src/Handler/messageHandler.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/home/alhuber/Schreibtisch/HTWSaar/Kommunikationsinformatik Master/3.Semester/VAA (1.Semester)/Uebungen/1.Uebung/repo/VAA/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_12) "Building CXX object CMakeFiles/graphGenApp.dir/src/Handler/messageHandler.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/graphGenApp.dir/src/Handler/messageHandler.cpp.o -c "/home/alhuber/Schreibtisch/HTWSaar/Kommunikationsinformatik Master/3.Semester/VAA (1.Semester)/Uebungen/1.Uebung/repo/VAA/src/Handler/messageHandler.cpp"

CMakeFiles/graphGenApp.dir/src/Handler/messageHandler.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/graphGenApp.dir/src/Handler/messageHandler.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/home/alhuber/Schreibtisch/HTWSaar/Kommunikationsinformatik Master/3.Semester/VAA (1.Semester)/Uebungen/1.Uebung/repo/VAA/src/Handler/messageHandler.cpp" > CMakeFiles/graphGenApp.dir/src/Handler/messageHandler.cpp.i

CMakeFiles/graphGenApp.dir/src/Handler/messageHandler.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/graphGenApp.dir/src/Handler/messageHandler.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/home/alhuber/Schreibtisch/HTWSaar/Kommunikationsinformatik Master/3.Semester/VAA (1.Semester)/Uebungen/1.Uebung/repo/VAA/src/Handler/messageHandler.cpp" -o CMakeFiles/graphGenApp.dir/src/Handler/messageHandler.cpp.s

# Object files for target graphGenApp
graphGenApp_OBJECTS = \
"CMakeFiles/graphGenApp.dir/src/Handler/floodHandler.cpp.o" \
"CMakeFiles/graphGenApp.dir/src/Handler/configHandler.cpp.o" \
"CMakeFiles/graphGenApp.dir/src/Graph/edge.cpp.o" \
"CMakeFiles/graphGenApp.dir/src/Handler/goldmanEdgeChasingHandler.cpp.o" \
"CMakeFiles/graphGenApp.dir/src/Application/agreement.cpp.o" \
"CMakeFiles/graphGenApp.dir/src/Graph/node.cpp.o" \
"CMakeFiles/graphGenApp.dir/src/Handler/fileHandler.cpp.o" \
"CMakeFiles/graphGenApp.dir/src/graphGenApp.cpp.o" \
"CMakeFiles/graphGenApp.dir/src/Handler/echoHandler.cpp.o" \
"CMakeFiles/graphGenApp.dir/src/Handler/accountHandler.cpp.o" \
"CMakeFiles/graphGenApp.dir/src/Application/doubleCounting.cpp.o" \
"CMakeFiles/graphGenApp.dir/src/Handler/messageHandler.cpp.o"

# External object files for target graphGenApp
graphGenApp_EXTERNAL_OBJECTS =

graphGenApp: CMakeFiles/graphGenApp.dir/src/Handler/floodHandler.cpp.o
graphGenApp: CMakeFiles/graphGenApp.dir/src/Handler/configHandler.cpp.o
graphGenApp: CMakeFiles/graphGenApp.dir/src/Graph/edge.cpp.o
graphGenApp: CMakeFiles/graphGenApp.dir/src/Handler/goldmanEdgeChasingHandler.cpp.o
graphGenApp: CMakeFiles/graphGenApp.dir/src/Application/agreement.cpp.o
graphGenApp: CMakeFiles/graphGenApp.dir/src/Graph/node.cpp.o
graphGenApp: CMakeFiles/graphGenApp.dir/src/Handler/fileHandler.cpp.o
graphGenApp: CMakeFiles/graphGenApp.dir/src/graphGenApp.cpp.o
graphGenApp: CMakeFiles/graphGenApp.dir/src/Handler/echoHandler.cpp.o
graphGenApp: CMakeFiles/graphGenApp.dir/src/Handler/accountHandler.cpp.o
graphGenApp: CMakeFiles/graphGenApp.dir/src/Application/doubleCounting.cpp.o
graphGenApp: CMakeFiles/graphGenApp.dir/src/Handler/messageHandler.cpp.o
graphGenApp: CMakeFiles/graphGenApp.dir/build.make
graphGenApp: CMakeFiles/graphGenApp.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="/home/alhuber/Schreibtisch/HTWSaar/Kommunikationsinformatik Master/3.Semester/VAA (1.Semester)/Uebungen/1.Uebung/repo/VAA/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_13) "Linking CXX executable graphGenApp"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/graphGenApp.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/graphGenApp.dir/build: graphGenApp

.PHONY : CMakeFiles/graphGenApp.dir/build

CMakeFiles/graphGenApp.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/graphGenApp.dir/cmake_clean.cmake
.PHONY : CMakeFiles/graphGenApp.dir/clean

CMakeFiles/graphGenApp.dir/depend:
	cd "/home/alhuber/Schreibtisch/HTWSaar/Kommunikationsinformatik Master/3.Semester/VAA (1.Semester)/Uebungen/1.Uebung/repo/VAA/cmake-build-debug" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/home/alhuber/Schreibtisch/HTWSaar/Kommunikationsinformatik Master/3.Semester/VAA (1.Semester)/Uebungen/1.Uebung/repo/VAA" "/home/alhuber/Schreibtisch/HTWSaar/Kommunikationsinformatik Master/3.Semester/VAA (1.Semester)/Uebungen/1.Uebung/repo/VAA" "/home/alhuber/Schreibtisch/HTWSaar/Kommunikationsinformatik Master/3.Semester/VAA (1.Semester)/Uebungen/1.Uebung/repo/VAA/cmake-build-debug" "/home/alhuber/Schreibtisch/HTWSaar/Kommunikationsinformatik Master/3.Semester/VAA (1.Semester)/Uebungen/1.Uebung/repo/VAA/cmake-build-debug" "/home/alhuber/Schreibtisch/HTWSaar/Kommunikationsinformatik Master/3.Semester/VAA (1.Semester)/Uebungen/1.Uebung/repo/VAA/cmake-build-debug/CMakeFiles/graphGenApp.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : CMakeFiles/graphGenApp.dir/depend

