# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.9

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
CMAKE_COMMAND = /usr/local/Cellar/cmake/3.9.4_1/bin/cmake

# The command to remove a file.
RM = /usr/local/Cellar/cmake/3.9.4_1/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/kushangupta/Documents/GraphicsFinalProj

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/kushangupta/Documents/GraphicsFinalProj/build

# Include any dependencies generated for this target.
include src/CMakeFiles/skinning.dir/depend.make

# Include the progress variables for this target.
include src/CMakeFiles/skinning.dir/progress.make

# Include the compile flags for this target's objects.
include src/CMakeFiles/skinning.dir/flags.make

src/CMakeFiles/skinning.dir/texture_to_render.cpp.o: src/CMakeFiles/skinning.dir/flags.make
src/CMakeFiles/skinning.dir/texture_to_render.cpp.o: ../src/texture_to_render.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/kushangupta/Documents/GraphicsFinalProj/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/CMakeFiles/skinning.dir/texture_to_render.cpp.o"
	cd /Users/kushangupta/Documents/GraphicsFinalProj/build/src && /Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/skinning.dir/texture_to_render.cpp.o -c /Users/kushangupta/Documents/GraphicsFinalProj/src/texture_to_render.cpp

src/CMakeFiles/skinning.dir/texture_to_render.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/skinning.dir/texture_to_render.cpp.i"
	cd /Users/kushangupta/Documents/GraphicsFinalProj/build/src && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/kushangupta/Documents/GraphicsFinalProj/src/texture_to_render.cpp > CMakeFiles/skinning.dir/texture_to_render.cpp.i

src/CMakeFiles/skinning.dir/texture_to_render.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/skinning.dir/texture_to_render.cpp.s"
	cd /Users/kushangupta/Documents/GraphicsFinalProj/build/src && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/kushangupta/Documents/GraphicsFinalProj/src/texture_to_render.cpp -o CMakeFiles/skinning.dir/texture_to_render.cpp.s

src/CMakeFiles/skinning.dir/texture_to_render.cpp.o.requires:

.PHONY : src/CMakeFiles/skinning.dir/texture_to_render.cpp.o.requires

src/CMakeFiles/skinning.dir/texture_to_render.cpp.o.provides: src/CMakeFiles/skinning.dir/texture_to_render.cpp.o.requires
	$(MAKE) -f src/CMakeFiles/skinning.dir/build.make src/CMakeFiles/skinning.dir/texture_to_render.cpp.o.provides.build
.PHONY : src/CMakeFiles/skinning.dir/texture_to_render.cpp.o.provides

src/CMakeFiles/skinning.dir/texture_to_render.cpp.o.provides.build: src/CMakeFiles/skinning.dir/texture_to_render.cpp.o


src/CMakeFiles/skinning.dir/RadiosityScene.cpp.o: src/CMakeFiles/skinning.dir/flags.make
src/CMakeFiles/skinning.dir/RadiosityScene.cpp.o: ../src/RadiosityScene.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/kushangupta/Documents/GraphicsFinalProj/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object src/CMakeFiles/skinning.dir/RadiosityScene.cpp.o"
	cd /Users/kushangupta/Documents/GraphicsFinalProj/build/src && /Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/skinning.dir/RadiosityScene.cpp.o -c /Users/kushangupta/Documents/GraphicsFinalProj/src/RadiosityScene.cpp

src/CMakeFiles/skinning.dir/RadiosityScene.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/skinning.dir/RadiosityScene.cpp.i"
	cd /Users/kushangupta/Documents/GraphicsFinalProj/build/src && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/kushangupta/Documents/GraphicsFinalProj/src/RadiosityScene.cpp > CMakeFiles/skinning.dir/RadiosityScene.cpp.i

src/CMakeFiles/skinning.dir/RadiosityScene.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/skinning.dir/RadiosityScene.cpp.s"
	cd /Users/kushangupta/Documents/GraphicsFinalProj/build/src && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/kushangupta/Documents/GraphicsFinalProj/src/RadiosityScene.cpp -o CMakeFiles/skinning.dir/RadiosityScene.cpp.s

src/CMakeFiles/skinning.dir/RadiosityScene.cpp.o.requires:

.PHONY : src/CMakeFiles/skinning.dir/RadiosityScene.cpp.o.requires

src/CMakeFiles/skinning.dir/RadiosityScene.cpp.o.provides: src/CMakeFiles/skinning.dir/RadiosityScene.cpp.o.requires
	$(MAKE) -f src/CMakeFiles/skinning.dir/build.make src/CMakeFiles/skinning.dir/RadiosityScene.cpp.o.provides.build
.PHONY : src/CMakeFiles/skinning.dir/RadiosityScene.cpp.o.provides

src/CMakeFiles/skinning.dir/RadiosityScene.cpp.o.provides.build: src/CMakeFiles/skinning.dir/RadiosityScene.cpp.o


src/CMakeFiles/skinning.dir/Cylinder.cpp.o: src/CMakeFiles/skinning.dir/flags.make
src/CMakeFiles/skinning.dir/Cylinder.cpp.o: ../src/Cylinder.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/kushangupta/Documents/GraphicsFinalProj/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object src/CMakeFiles/skinning.dir/Cylinder.cpp.o"
	cd /Users/kushangupta/Documents/GraphicsFinalProj/build/src && /Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/skinning.dir/Cylinder.cpp.o -c /Users/kushangupta/Documents/GraphicsFinalProj/src/Cylinder.cpp

src/CMakeFiles/skinning.dir/Cylinder.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/skinning.dir/Cylinder.cpp.i"
	cd /Users/kushangupta/Documents/GraphicsFinalProj/build/src && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/kushangupta/Documents/GraphicsFinalProj/src/Cylinder.cpp > CMakeFiles/skinning.dir/Cylinder.cpp.i

src/CMakeFiles/skinning.dir/Cylinder.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/skinning.dir/Cylinder.cpp.s"
	cd /Users/kushangupta/Documents/GraphicsFinalProj/build/src && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/kushangupta/Documents/GraphicsFinalProj/src/Cylinder.cpp -o CMakeFiles/skinning.dir/Cylinder.cpp.s

src/CMakeFiles/skinning.dir/Cylinder.cpp.o.requires:

.PHONY : src/CMakeFiles/skinning.dir/Cylinder.cpp.o.requires

src/CMakeFiles/skinning.dir/Cylinder.cpp.o.provides: src/CMakeFiles/skinning.dir/Cylinder.cpp.o.requires
	$(MAKE) -f src/CMakeFiles/skinning.dir/build.make src/CMakeFiles/skinning.dir/Cylinder.cpp.o.provides.build
.PHONY : src/CMakeFiles/skinning.dir/Cylinder.cpp.o.provides

src/CMakeFiles/skinning.dir/Cylinder.cpp.o.provides.build: src/CMakeFiles/skinning.dir/Cylinder.cpp.o


src/CMakeFiles/skinning.dir/animation_loader_saver.cc.o: src/CMakeFiles/skinning.dir/flags.make
src/CMakeFiles/skinning.dir/animation_loader_saver.cc.o: ../src/animation_loader_saver.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/kushangupta/Documents/GraphicsFinalProj/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object src/CMakeFiles/skinning.dir/animation_loader_saver.cc.o"
	cd /Users/kushangupta/Documents/GraphicsFinalProj/build/src && /Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/skinning.dir/animation_loader_saver.cc.o -c /Users/kushangupta/Documents/GraphicsFinalProj/src/animation_loader_saver.cc

src/CMakeFiles/skinning.dir/animation_loader_saver.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/skinning.dir/animation_loader_saver.cc.i"
	cd /Users/kushangupta/Documents/GraphicsFinalProj/build/src && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/kushangupta/Documents/GraphicsFinalProj/src/animation_loader_saver.cc > CMakeFiles/skinning.dir/animation_loader_saver.cc.i

src/CMakeFiles/skinning.dir/animation_loader_saver.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/skinning.dir/animation_loader_saver.cc.s"
	cd /Users/kushangupta/Documents/GraphicsFinalProj/build/src && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/kushangupta/Documents/GraphicsFinalProj/src/animation_loader_saver.cc -o CMakeFiles/skinning.dir/animation_loader_saver.cc.s

src/CMakeFiles/skinning.dir/animation_loader_saver.cc.o.requires:

.PHONY : src/CMakeFiles/skinning.dir/animation_loader_saver.cc.o.requires

src/CMakeFiles/skinning.dir/animation_loader_saver.cc.o.provides: src/CMakeFiles/skinning.dir/animation_loader_saver.cc.o.requires
	$(MAKE) -f src/CMakeFiles/skinning.dir/build.make src/CMakeFiles/skinning.dir/animation_loader_saver.cc.o.provides.build
.PHONY : src/CMakeFiles/skinning.dir/animation_loader_saver.cc.o.provides

src/CMakeFiles/skinning.dir/animation_loader_saver.cc.o.provides.build: src/CMakeFiles/skinning.dir/animation_loader_saver.cc.o


src/CMakeFiles/skinning.dir/bbox.cc.o: src/CMakeFiles/skinning.dir/flags.make
src/CMakeFiles/skinning.dir/bbox.cc.o: ../src/bbox.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/kushangupta/Documents/GraphicsFinalProj/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object src/CMakeFiles/skinning.dir/bbox.cc.o"
	cd /Users/kushangupta/Documents/GraphicsFinalProj/build/src && /Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/skinning.dir/bbox.cc.o -c /Users/kushangupta/Documents/GraphicsFinalProj/src/bbox.cc

src/CMakeFiles/skinning.dir/bbox.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/skinning.dir/bbox.cc.i"
	cd /Users/kushangupta/Documents/GraphicsFinalProj/build/src && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/kushangupta/Documents/GraphicsFinalProj/src/bbox.cc > CMakeFiles/skinning.dir/bbox.cc.i

src/CMakeFiles/skinning.dir/bbox.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/skinning.dir/bbox.cc.s"
	cd /Users/kushangupta/Documents/GraphicsFinalProj/build/src && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/kushangupta/Documents/GraphicsFinalProj/src/bbox.cc -o CMakeFiles/skinning.dir/bbox.cc.s

src/CMakeFiles/skinning.dir/bbox.cc.o.requires:

.PHONY : src/CMakeFiles/skinning.dir/bbox.cc.o.requires

src/CMakeFiles/skinning.dir/bbox.cc.o.provides: src/CMakeFiles/skinning.dir/bbox.cc.o.requires
	$(MAKE) -f src/CMakeFiles/skinning.dir/build.make src/CMakeFiles/skinning.dir/bbox.cc.o.provides.build
.PHONY : src/CMakeFiles/skinning.dir/bbox.cc.o.provides

src/CMakeFiles/skinning.dir/bbox.cc.o.provides.build: src/CMakeFiles/skinning.dir/bbox.cc.o


src/CMakeFiles/skinning.dir/bone_geometry.cc.o: src/CMakeFiles/skinning.dir/flags.make
src/CMakeFiles/skinning.dir/bone_geometry.cc.o: ../src/bone_geometry.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/kushangupta/Documents/GraphicsFinalProj/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object src/CMakeFiles/skinning.dir/bone_geometry.cc.o"
	cd /Users/kushangupta/Documents/GraphicsFinalProj/build/src && /Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/skinning.dir/bone_geometry.cc.o -c /Users/kushangupta/Documents/GraphicsFinalProj/src/bone_geometry.cc

src/CMakeFiles/skinning.dir/bone_geometry.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/skinning.dir/bone_geometry.cc.i"
	cd /Users/kushangupta/Documents/GraphicsFinalProj/build/src && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/kushangupta/Documents/GraphicsFinalProj/src/bone_geometry.cc > CMakeFiles/skinning.dir/bone_geometry.cc.i

src/CMakeFiles/skinning.dir/bone_geometry.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/skinning.dir/bone_geometry.cc.s"
	cd /Users/kushangupta/Documents/GraphicsFinalProj/build/src && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/kushangupta/Documents/GraphicsFinalProj/src/bone_geometry.cc -o CMakeFiles/skinning.dir/bone_geometry.cc.s

src/CMakeFiles/skinning.dir/bone_geometry.cc.o.requires:

.PHONY : src/CMakeFiles/skinning.dir/bone_geometry.cc.o.requires

src/CMakeFiles/skinning.dir/bone_geometry.cc.o.provides: src/CMakeFiles/skinning.dir/bone_geometry.cc.o.requires
	$(MAKE) -f src/CMakeFiles/skinning.dir/build.make src/CMakeFiles/skinning.dir/bone_geometry.cc.o.provides.build
.PHONY : src/CMakeFiles/skinning.dir/bone_geometry.cc.o.provides

src/CMakeFiles/skinning.dir/bone_geometry.cc.o.provides.build: src/CMakeFiles/skinning.dir/bone_geometry.cc.o


src/CMakeFiles/skinning.dir/gui.cc.o: src/CMakeFiles/skinning.dir/flags.make
src/CMakeFiles/skinning.dir/gui.cc.o: ../src/gui.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/kushangupta/Documents/GraphicsFinalProj/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object src/CMakeFiles/skinning.dir/gui.cc.o"
	cd /Users/kushangupta/Documents/GraphicsFinalProj/build/src && /Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/skinning.dir/gui.cc.o -c /Users/kushangupta/Documents/GraphicsFinalProj/src/gui.cc

src/CMakeFiles/skinning.dir/gui.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/skinning.dir/gui.cc.i"
	cd /Users/kushangupta/Documents/GraphicsFinalProj/build/src && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/kushangupta/Documents/GraphicsFinalProj/src/gui.cc > CMakeFiles/skinning.dir/gui.cc.i

src/CMakeFiles/skinning.dir/gui.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/skinning.dir/gui.cc.s"
	cd /Users/kushangupta/Documents/GraphicsFinalProj/build/src && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/kushangupta/Documents/GraphicsFinalProj/src/gui.cc -o CMakeFiles/skinning.dir/gui.cc.s

src/CMakeFiles/skinning.dir/gui.cc.o.requires:

.PHONY : src/CMakeFiles/skinning.dir/gui.cc.o.requires

src/CMakeFiles/skinning.dir/gui.cc.o.provides: src/CMakeFiles/skinning.dir/gui.cc.o.requires
	$(MAKE) -f src/CMakeFiles/skinning.dir/build.make src/CMakeFiles/skinning.dir/gui.cc.o.provides.build
.PHONY : src/CMakeFiles/skinning.dir/gui.cc.o.provides

src/CMakeFiles/skinning.dir/gui.cc.o.provides.build: src/CMakeFiles/skinning.dir/gui.cc.o


src/CMakeFiles/skinning.dir/kdTree.cpp.o: src/CMakeFiles/skinning.dir/flags.make
src/CMakeFiles/skinning.dir/kdTree.cpp.o: ../src/kdTree.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/kushangupta/Documents/GraphicsFinalProj/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object src/CMakeFiles/skinning.dir/kdTree.cpp.o"
	cd /Users/kushangupta/Documents/GraphicsFinalProj/build/src && /Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/skinning.dir/kdTree.cpp.o -c /Users/kushangupta/Documents/GraphicsFinalProj/src/kdTree.cpp

src/CMakeFiles/skinning.dir/kdTree.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/skinning.dir/kdTree.cpp.i"
	cd /Users/kushangupta/Documents/GraphicsFinalProj/build/src && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/kushangupta/Documents/GraphicsFinalProj/src/kdTree.cpp > CMakeFiles/skinning.dir/kdTree.cpp.i

src/CMakeFiles/skinning.dir/kdTree.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/skinning.dir/kdTree.cpp.s"
	cd /Users/kushangupta/Documents/GraphicsFinalProj/build/src && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/kushangupta/Documents/GraphicsFinalProj/src/kdTree.cpp -o CMakeFiles/skinning.dir/kdTree.cpp.s

src/CMakeFiles/skinning.dir/kdTree.cpp.o.requires:

.PHONY : src/CMakeFiles/skinning.dir/kdTree.cpp.o.requires

src/CMakeFiles/skinning.dir/kdTree.cpp.o.provides: src/CMakeFiles/skinning.dir/kdTree.cpp.o.requires
	$(MAKE) -f src/CMakeFiles/skinning.dir/build.make src/CMakeFiles/skinning.dir/kdTree.cpp.o.provides.build
.PHONY : src/CMakeFiles/skinning.dir/kdTree.cpp.o.provides

src/CMakeFiles/skinning.dir/kdTree.cpp.o.provides.build: src/CMakeFiles/skinning.dir/kdTree.cpp.o


src/CMakeFiles/skinning.dir/main.cc.o: src/CMakeFiles/skinning.dir/flags.make
src/CMakeFiles/skinning.dir/main.cc.o: ../src/main.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/kushangupta/Documents/GraphicsFinalProj/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object src/CMakeFiles/skinning.dir/main.cc.o"
	cd /Users/kushangupta/Documents/GraphicsFinalProj/build/src && /Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/skinning.dir/main.cc.o -c /Users/kushangupta/Documents/GraphicsFinalProj/src/main.cc

src/CMakeFiles/skinning.dir/main.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/skinning.dir/main.cc.i"
	cd /Users/kushangupta/Documents/GraphicsFinalProj/build/src && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/kushangupta/Documents/GraphicsFinalProj/src/main.cc > CMakeFiles/skinning.dir/main.cc.i

src/CMakeFiles/skinning.dir/main.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/skinning.dir/main.cc.s"
	cd /Users/kushangupta/Documents/GraphicsFinalProj/build/src && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/kushangupta/Documents/GraphicsFinalProj/src/main.cc -o CMakeFiles/skinning.dir/main.cc.s

src/CMakeFiles/skinning.dir/main.cc.o.requires:

.PHONY : src/CMakeFiles/skinning.dir/main.cc.o.requires

src/CMakeFiles/skinning.dir/main.cc.o.provides: src/CMakeFiles/skinning.dir/main.cc.o.requires
	$(MAKE) -f src/CMakeFiles/skinning.dir/build.make src/CMakeFiles/skinning.dir/main.cc.o.provides.build
.PHONY : src/CMakeFiles/skinning.dir/main.cc.o.provides

src/CMakeFiles/skinning.dir/main.cc.o.provides.build: src/CMakeFiles/skinning.dir/main.cc.o


src/CMakeFiles/skinning.dir/procedure_geometry.cc.o: src/CMakeFiles/skinning.dir/flags.make
src/CMakeFiles/skinning.dir/procedure_geometry.cc.o: ../src/procedure_geometry.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/kushangupta/Documents/GraphicsFinalProj/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Building CXX object src/CMakeFiles/skinning.dir/procedure_geometry.cc.o"
	cd /Users/kushangupta/Documents/GraphicsFinalProj/build/src && /Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/skinning.dir/procedure_geometry.cc.o -c /Users/kushangupta/Documents/GraphicsFinalProj/src/procedure_geometry.cc

src/CMakeFiles/skinning.dir/procedure_geometry.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/skinning.dir/procedure_geometry.cc.i"
	cd /Users/kushangupta/Documents/GraphicsFinalProj/build/src && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/kushangupta/Documents/GraphicsFinalProj/src/procedure_geometry.cc > CMakeFiles/skinning.dir/procedure_geometry.cc.i

src/CMakeFiles/skinning.dir/procedure_geometry.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/skinning.dir/procedure_geometry.cc.s"
	cd /Users/kushangupta/Documents/GraphicsFinalProj/build/src && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/kushangupta/Documents/GraphicsFinalProj/src/procedure_geometry.cc -o CMakeFiles/skinning.dir/procedure_geometry.cc.s

src/CMakeFiles/skinning.dir/procedure_geometry.cc.o.requires:

.PHONY : src/CMakeFiles/skinning.dir/procedure_geometry.cc.o.requires

src/CMakeFiles/skinning.dir/procedure_geometry.cc.o.provides: src/CMakeFiles/skinning.dir/procedure_geometry.cc.o.requires
	$(MAKE) -f src/CMakeFiles/skinning.dir/build.make src/CMakeFiles/skinning.dir/procedure_geometry.cc.o.provides.build
.PHONY : src/CMakeFiles/skinning.dir/procedure_geometry.cc.o.provides

src/CMakeFiles/skinning.dir/procedure_geometry.cc.o.provides.build: src/CMakeFiles/skinning.dir/procedure_geometry.cc.o


src/CMakeFiles/skinning.dir/ray.cpp.o: src/CMakeFiles/skinning.dir/flags.make
src/CMakeFiles/skinning.dir/ray.cpp.o: ../src/ray.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/kushangupta/Documents/GraphicsFinalProj/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Building CXX object src/CMakeFiles/skinning.dir/ray.cpp.o"
	cd /Users/kushangupta/Documents/GraphicsFinalProj/build/src && /Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/skinning.dir/ray.cpp.o -c /Users/kushangupta/Documents/GraphicsFinalProj/src/ray.cpp

src/CMakeFiles/skinning.dir/ray.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/skinning.dir/ray.cpp.i"
	cd /Users/kushangupta/Documents/GraphicsFinalProj/build/src && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/kushangupta/Documents/GraphicsFinalProj/src/ray.cpp > CMakeFiles/skinning.dir/ray.cpp.i

src/CMakeFiles/skinning.dir/ray.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/skinning.dir/ray.cpp.s"
	cd /Users/kushangupta/Documents/GraphicsFinalProj/build/src && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/kushangupta/Documents/GraphicsFinalProj/src/ray.cpp -o CMakeFiles/skinning.dir/ray.cpp.s

src/CMakeFiles/skinning.dir/ray.cpp.o.requires:

.PHONY : src/CMakeFiles/skinning.dir/ray.cpp.o.requires

src/CMakeFiles/skinning.dir/ray.cpp.o.provides: src/CMakeFiles/skinning.dir/ray.cpp.o.requires
	$(MAKE) -f src/CMakeFiles/skinning.dir/build.make src/CMakeFiles/skinning.dir/ray.cpp.o.provides.build
.PHONY : src/CMakeFiles/skinning.dir/ray.cpp.o.provides

src/CMakeFiles/skinning.dir/ray.cpp.o.provides.build: src/CMakeFiles/skinning.dir/ray.cpp.o


src/CMakeFiles/skinning.dir/render_pass.cc.o: src/CMakeFiles/skinning.dir/flags.make
src/CMakeFiles/skinning.dir/render_pass.cc.o: ../src/render_pass.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/kushangupta/Documents/GraphicsFinalProj/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_12) "Building CXX object src/CMakeFiles/skinning.dir/render_pass.cc.o"
	cd /Users/kushangupta/Documents/GraphicsFinalProj/build/src && /Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/skinning.dir/render_pass.cc.o -c /Users/kushangupta/Documents/GraphicsFinalProj/src/render_pass.cc

src/CMakeFiles/skinning.dir/render_pass.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/skinning.dir/render_pass.cc.i"
	cd /Users/kushangupta/Documents/GraphicsFinalProj/build/src && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/kushangupta/Documents/GraphicsFinalProj/src/render_pass.cc > CMakeFiles/skinning.dir/render_pass.cc.i

src/CMakeFiles/skinning.dir/render_pass.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/skinning.dir/render_pass.cc.s"
	cd /Users/kushangupta/Documents/GraphicsFinalProj/build/src && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/kushangupta/Documents/GraphicsFinalProj/src/render_pass.cc -o CMakeFiles/skinning.dir/render_pass.cc.s

src/CMakeFiles/skinning.dir/render_pass.cc.o.requires:

.PHONY : src/CMakeFiles/skinning.dir/render_pass.cc.o.requires

src/CMakeFiles/skinning.dir/render_pass.cc.o.provides: src/CMakeFiles/skinning.dir/render_pass.cc.o.requires
	$(MAKE) -f src/CMakeFiles/skinning.dir/build.make src/CMakeFiles/skinning.dir/render_pass.cc.o.provides.build
.PHONY : src/CMakeFiles/skinning.dir/render_pass.cc.o.provides

src/CMakeFiles/skinning.dir/render_pass.cc.o.provides.build: src/CMakeFiles/skinning.dir/render_pass.cc.o


src/CMakeFiles/skinning.dir/scene.cpp.o: src/CMakeFiles/skinning.dir/flags.make
src/CMakeFiles/skinning.dir/scene.cpp.o: ../src/scene.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/kushangupta/Documents/GraphicsFinalProj/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_13) "Building CXX object src/CMakeFiles/skinning.dir/scene.cpp.o"
	cd /Users/kushangupta/Documents/GraphicsFinalProj/build/src && /Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/skinning.dir/scene.cpp.o -c /Users/kushangupta/Documents/GraphicsFinalProj/src/scene.cpp

src/CMakeFiles/skinning.dir/scene.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/skinning.dir/scene.cpp.i"
	cd /Users/kushangupta/Documents/GraphicsFinalProj/build/src && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/kushangupta/Documents/GraphicsFinalProj/src/scene.cpp > CMakeFiles/skinning.dir/scene.cpp.i

src/CMakeFiles/skinning.dir/scene.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/skinning.dir/scene.cpp.s"
	cd /Users/kushangupta/Documents/GraphicsFinalProj/build/src && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/kushangupta/Documents/GraphicsFinalProj/src/scene.cpp -o CMakeFiles/skinning.dir/scene.cpp.s

src/CMakeFiles/skinning.dir/scene.cpp.o.requires:

.PHONY : src/CMakeFiles/skinning.dir/scene.cpp.o.requires

src/CMakeFiles/skinning.dir/scene.cpp.o.provides: src/CMakeFiles/skinning.dir/scene.cpp.o.requires
	$(MAKE) -f src/CMakeFiles/skinning.dir/build.make src/CMakeFiles/skinning.dir/scene.cpp.o.provides.build
.PHONY : src/CMakeFiles/skinning.dir/scene.cpp.o.provides

src/CMakeFiles/skinning.dir/scene.cpp.o.provides.build: src/CMakeFiles/skinning.dir/scene.cpp.o


src/CMakeFiles/skinning.dir/shader_uniform.cc.o: src/CMakeFiles/skinning.dir/flags.make
src/CMakeFiles/skinning.dir/shader_uniform.cc.o: ../src/shader_uniform.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/kushangupta/Documents/GraphicsFinalProj/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_14) "Building CXX object src/CMakeFiles/skinning.dir/shader_uniform.cc.o"
	cd /Users/kushangupta/Documents/GraphicsFinalProj/build/src && /Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/skinning.dir/shader_uniform.cc.o -c /Users/kushangupta/Documents/GraphicsFinalProj/src/shader_uniform.cc

src/CMakeFiles/skinning.dir/shader_uniform.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/skinning.dir/shader_uniform.cc.i"
	cd /Users/kushangupta/Documents/GraphicsFinalProj/build/src && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/kushangupta/Documents/GraphicsFinalProj/src/shader_uniform.cc > CMakeFiles/skinning.dir/shader_uniform.cc.i

src/CMakeFiles/skinning.dir/shader_uniform.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/skinning.dir/shader_uniform.cc.s"
	cd /Users/kushangupta/Documents/GraphicsFinalProj/build/src && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/kushangupta/Documents/GraphicsFinalProj/src/shader_uniform.cc -o CMakeFiles/skinning.dir/shader_uniform.cc.s

src/CMakeFiles/skinning.dir/shader_uniform.cc.o.requires:

.PHONY : src/CMakeFiles/skinning.dir/shader_uniform.cc.o.requires

src/CMakeFiles/skinning.dir/shader_uniform.cc.o.provides: src/CMakeFiles/skinning.dir/shader_uniform.cc.o.requires
	$(MAKE) -f src/CMakeFiles/skinning.dir/build.make src/CMakeFiles/skinning.dir/shader_uniform.cc.o.provides.build
.PHONY : src/CMakeFiles/skinning.dir/shader_uniform.cc.o.provides

src/CMakeFiles/skinning.dir/shader_uniform.cc.o.provides.build: src/CMakeFiles/skinning.dir/shader_uniform.cc.o


src/CMakeFiles/skinning.dir/tinyfiledialogs.c.o: src/CMakeFiles/skinning.dir/flags.make
src/CMakeFiles/skinning.dir/tinyfiledialogs.c.o: ../src/tinyfiledialogs.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/kushangupta/Documents/GraphicsFinalProj/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_15) "Building C object src/CMakeFiles/skinning.dir/tinyfiledialogs.c.o"
	cd /Users/kushangupta/Documents/GraphicsFinalProj/build/src && /Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/skinning.dir/tinyfiledialogs.c.o   -c /Users/kushangupta/Documents/GraphicsFinalProj/src/tinyfiledialogs.c

src/CMakeFiles/skinning.dir/tinyfiledialogs.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/skinning.dir/tinyfiledialogs.c.i"
	cd /Users/kushangupta/Documents/GraphicsFinalProj/build/src && /Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/kushangupta/Documents/GraphicsFinalProj/src/tinyfiledialogs.c > CMakeFiles/skinning.dir/tinyfiledialogs.c.i

src/CMakeFiles/skinning.dir/tinyfiledialogs.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/skinning.dir/tinyfiledialogs.c.s"
	cd /Users/kushangupta/Documents/GraphicsFinalProj/build/src && /Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/kushangupta/Documents/GraphicsFinalProj/src/tinyfiledialogs.c -o CMakeFiles/skinning.dir/tinyfiledialogs.c.s

src/CMakeFiles/skinning.dir/tinyfiledialogs.c.o.requires:

.PHONY : src/CMakeFiles/skinning.dir/tinyfiledialogs.c.o.requires

src/CMakeFiles/skinning.dir/tinyfiledialogs.c.o.provides: src/CMakeFiles/skinning.dir/tinyfiledialogs.c.o.requires
	$(MAKE) -f src/CMakeFiles/skinning.dir/build.make src/CMakeFiles/skinning.dir/tinyfiledialogs.c.o.provides.build
.PHONY : src/CMakeFiles/skinning.dir/tinyfiledialogs.c.o.provides

src/CMakeFiles/skinning.dir/tinyfiledialogs.c.o.provides.build: src/CMakeFiles/skinning.dir/tinyfiledialogs.c.o


# Object files for target skinning
skinning_OBJECTS = \
"CMakeFiles/skinning.dir/texture_to_render.cpp.o" \
"CMakeFiles/skinning.dir/RadiosityScene.cpp.o" \
"CMakeFiles/skinning.dir/Cylinder.cpp.o" \
"CMakeFiles/skinning.dir/animation_loader_saver.cc.o" \
"CMakeFiles/skinning.dir/bbox.cc.o" \
"CMakeFiles/skinning.dir/bone_geometry.cc.o" \
"CMakeFiles/skinning.dir/gui.cc.o" \
"CMakeFiles/skinning.dir/kdTree.cpp.o" \
"CMakeFiles/skinning.dir/main.cc.o" \
"CMakeFiles/skinning.dir/procedure_geometry.cc.o" \
"CMakeFiles/skinning.dir/ray.cpp.o" \
"CMakeFiles/skinning.dir/render_pass.cc.o" \
"CMakeFiles/skinning.dir/scene.cpp.o" \
"CMakeFiles/skinning.dir/shader_uniform.cc.o" \
"CMakeFiles/skinning.dir/tinyfiledialogs.c.o"

# External object files for target skinning
skinning_EXTERNAL_OBJECTS =

bin/skinning: src/CMakeFiles/skinning.dir/texture_to_render.cpp.o
bin/skinning: src/CMakeFiles/skinning.dir/RadiosityScene.cpp.o
bin/skinning: src/CMakeFiles/skinning.dir/Cylinder.cpp.o
bin/skinning: src/CMakeFiles/skinning.dir/animation_loader_saver.cc.o
bin/skinning: src/CMakeFiles/skinning.dir/bbox.cc.o
bin/skinning: src/CMakeFiles/skinning.dir/bone_geometry.cc.o
bin/skinning: src/CMakeFiles/skinning.dir/gui.cc.o
bin/skinning: src/CMakeFiles/skinning.dir/kdTree.cpp.o
bin/skinning: src/CMakeFiles/skinning.dir/main.cc.o
bin/skinning: src/CMakeFiles/skinning.dir/procedure_geometry.cc.o
bin/skinning: src/CMakeFiles/skinning.dir/ray.cpp.o
bin/skinning: src/CMakeFiles/skinning.dir/render_pass.cc.o
bin/skinning: src/CMakeFiles/skinning.dir/scene.cpp.o
bin/skinning: src/CMakeFiles/skinning.dir/shader_uniform.cc.o
bin/skinning: src/CMakeFiles/skinning.dir/tinyfiledialogs.c.o
bin/skinning: src/CMakeFiles/skinning.dir/build.make
bin/skinning: /usr/local/lib/libGLEW.dylib
bin/skinning: /usr/local/lib/libGLEW.dylib
bin/skinning: libutgraphicsutil.a
bin/skinning: /usr/local/lib/libjpeg.dylib
bin/skinning: libpmdreader.a
bin/skinning: /usr/local/lib/libGLEW.dylib
bin/skinning: src/CMakeFiles/skinning.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/kushangupta/Documents/GraphicsFinalProj/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_16) "Linking CXX executable ../bin/skinning"
	cd /Users/kushangupta/Documents/GraphicsFinalProj/build/src && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/skinning.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/CMakeFiles/skinning.dir/build: bin/skinning

.PHONY : src/CMakeFiles/skinning.dir/build

src/CMakeFiles/skinning.dir/requires: src/CMakeFiles/skinning.dir/texture_to_render.cpp.o.requires
src/CMakeFiles/skinning.dir/requires: src/CMakeFiles/skinning.dir/RadiosityScene.cpp.o.requires
src/CMakeFiles/skinning.dir/requires: src/CMakeFiles/skinning.dir/Cylinder.cpp.o.requires
src/CMakeFiles/skinning.dir/requires: src/CMakeFiles/skinning.dir/animation_loader_saver.cc.o.requires
src/CMakeFiles/skinning.dir/requires: src/CMakeFiles/skinning.dir/bbox.cc.o.requires
src/CMakeFiles/skinning.dir/requires: src/CMakeFiles/skinning.dir/bone_geometry.cc.o.requires
src/CMakeFiles/skinning.dir/requires: src/CMakeFiles/skinning.dir/gui.cc.o.requires
src/CMakeFiles/skinning.dir/requires: src/CMakeFiles/skinning.dir/kdTree.cpp.o.requires
src/CMakeFiles/skinning.dir/requires: src/CMakeFiles/skinning.dir/main.cc.o.requires
src/CMakeFiles/skinning.dir/requires: src/CMakeFiles/skinning.dir/procedure_geometry.cc.o.requires
src/CMakeFiles/skinning.dir/requires: src/CMakeFiles/skinning.dir/ray.cpp.o.requires
src/CMakeFiles/skinning.dir/requires: src/CMakeFiles/skinning.dir/render_pass.cc.o.requires
src/CMakeFiles/skinning.dir/requires: src/CMakeFiles/skinning.dir/scene.cpp.o.requires
src/CMakeFiles/skinning.dir/requires: src/CMakeFiles/skinning.dir/shader_uniform.cc.o.requires
src/CMakeFiles/skinning.dir/requires: src/CMakeFiles/skinning.dir/tinyfiledialogs.c.o.requires

.PHONY : src/CMakeFiles/skinning.dir/requires

src/CMakeFiles/skinning.dir/clean:
	cd /Users/kushangupta/Documents/GraphicsFinalProj/build/src && $(CMAKE_COMMAND) -P CMakeFiles/skinning.dir/cmake_clean.cmake
.PHONY : src/CMakeFiles/skinning.dir/clean

src/CMakeFiles/skinning.dir/depend:
	cd /Users/kushangupta/Documents/GraphicsFinalProj/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/kushangupta/Documents/GraphicsFinalProj /Users/kushangupta/Documents/GraphicsFinalProj/src /Users/kushangupta/Documents/GraphicsFinalProj/build /Users/kushangupta/Documents/GraphicsFinalProj/build/src /Users/kushangupta/Documents/GraphicsFinalProj/build/src/CMakeFiles/skinning.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/CMakeFiles/skinning.dir/depend
