# CMAKE generated file: DO NOT EDIT!
# Generated by "NMake Makefiles" Generator, CMake Version 2.8

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canoncical targets will work.
.SUFFIXES:

.SUFFIXES: .hpux_make_needs_suffix_list

# Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE
NULL=nul
!ENDIF
SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files (x86)\CMake 2.8\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files (x86)\CMake 2.8\bin\cmake.exe" -E remove -f

# The program to use to edit the cache.
CMAKE_EDIT_COMMAND = "C:\Program Files (x86)\CMake 2.8\bin\cmake-gui.exe"

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = X:\Swift\Source\Engine\lib\zlib-1.2.8

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = X:\Swift\Source\Engine\lib\zlib-1.2.8\build-release

# Include any dependencies generated for this target.
include CMakeFiles\minigzip.dir\depend.make

# Include the progress variables for this target.
include CMakeFiles\minigzip.dir\progress.make

# Include the compile flags for this target's objects.
include CMakeFiles\minigzip.dir\flags.make

CMakeFiles\minigzip.dir\test\minigzip.obj: CMakeFiles\minigzip.dir\flags.make
CMakeFiles\minigzip.dir\test\minigzip.obj: ..\test\minigzip.c
	$(CMAKE_COMMAND) -E cmake_progress_report X:\Swift\Source\Engine\lib\zlib-1.2.8\build-release\CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object CMakeFiles/minigzip.dir/test/minigzip.obj"
	C:\PROGRA~2\MICROS~3.0\VC\bin\cl.exe  @<<
 /nologo $(C_FLAGS) $(C_DEFINES) /FoCMakeFiles\minigzip.dir\test\minigzip.obj /FdX:\Swift\Source\Engine\lib\zlib-1.2.8\build-release\minigzip.pdb -c X:\Swift\Source\Engine\lib\zlib-1.2.8\test\minigzip.c
<<

CMakeFiles\minigzip.dir\test\minigzip.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/minigzip.dir/test/minigzip.i"
	C:\PROGRA~2\MICROS~3.0\VC\bin\cl.exe  > CMakeFiles\minigzip.dir\test\minigzip.i @<<
 /nologo $(C_FLAGS) $(C_DEFINES) -E X:\Swift\Source\Engine\lib\zlib-1.2.8\test\minigzip.c
<<

CMakeFiles\minigzip.dir\test\minigzip.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/minigzip.dir/test/minigzip.s"
	C:\PROGRA~2\MICROS~3.0\VC\bin\cl.exe  @<<
 /nologo $(C_FLAGS) /FAs /FoNUL /FaCMakeFiles\minigzip.dir\test\minigzip.s /c X:\Swift\Source\Engine\lib\zlib-1.2.8\test\minigzip.c
<<

CMakeFiles\minigzip.dir\test\minigzip.obj.requires:
.PHONY : CMakeFiles\minigzip.dir\test\minigzip.obj.requires

CMakeFiles\minigzip.dir\test\minigzip.obj.provides: CMakeFiles\minigzip.dir\test\minigzip.obj.requires
	$(MAKE) -f CMakeFiles\minigzip.dir\build.make /nologo -$(MAKEFLAGS) CMakeFiles\minigzip.dir\test\minigzip.obj.provides.build
.PHONY : CMakeFiles\minigzip.dir\test\minigzip.obj.provides

CMakeFiles\minigzip.dir\test\minigzip.obj.provides.build: CMakeFiles\minigzip.dir\test\minigzip.obj

# Object files for target minigzip
minigzip_OBJECTS = \
"CMakeFiles\minigzip.dir\test\minigzip.obj"

# External object files for target minigzip
minigzip_EXTERNAL_OBJECTS =

minigzip.exe: CMakeFiles\minigzip.dir\test\minigzip.obj
minigzip.exe: zlib.lib
minigzip.exe: CMakeFiles\minigzip.dir\build.make
minigzip.exe: CMakeFiles\minigzip.dir\objects1.rsp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking C executable minigzip.exe"
	"C:\Program Files (x86)\CMake 2.8\bin\cmake.exe" -E vs_link_exe C:\PROGRA~2\MICROS~3.0\VC\bin\cl.exe  /nologo @CMakeFiles\minigzip.dir\objects1.rsp @<<
  /DWIN32 /D_WINDOWS /W3 /Zm1000 /MT /O2 /Ob2 /D NDEBUG /Feminigzip.exe /FdX:\Swift\Source\Engine\lib\zlib-1.2.8\build-release\minigzip.pdb -link /implib:minigzip.lib /version:0.0   /STACK:10000000 /machine:X86  /INCREMENTAL:NO /subsystem:console  zlib.lib kernel32.lib user32.lib gdi32.lib winspool.lib shell32.lib ole32.lib oleaut32.lib uuid.lib comdlg32.lib advapi32.lib  
<<

# Rule to build all files generated by this target.
CMakeFiles\minigzip.dir\build: minigzip.exe
.PHONY : CMakeFiles\minigzip.dir\build

CMakeFiles\minigzip.dir\requires: CMakeFiles\minigzip.dir\test\minigzip.obj.requires
.PHONY : CMakeFiles\minigzip.dir\requires

CMakeFiles\minigzip.dir\clean:
	$(CMAKE_COMMAND) -P CMakeFiles\minigzip.dir\cmake_clean.cmake
.PHONY : CMakeFiles\minigzip.dir\clean

CMakeFiles\minigzip.dir\depend:
	$(CMAKE_COMMAND) -E cmake_depends "NMake Makefiles" X:\Swift\Source\Engine\lib\zlib-1.2.8 X:\Swift\Source\Engine\lib\zlib-1.2.8 X:\Swift\Source\Engine\lib\zlib-1.2.8\build-release X:\Swift\Source\Engine\lib\zlib-1.2.8\build-release X:\Swift\Source\Engine\lib\zlib-1.2.8\build-release\CMakeFiles\minigzip.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles\minigzip.dir\depend

