cmake_minimum_required(VERSION 3.12)
project(cpplua)

set(CMAKE_CXX_STANDARD 11)

#set(SOURCE_FILES main.cpp lua/ltablib.c lua/lauxlib.c lua/lbaselib.c lua/lapi.c lua/lbitlib.c lua/lcode.c lua/lcorolib.c lua/lctype.c lua/ldblib.c lua/ldebug.c lua/ldo.c lua/ldump.c lua/lfunc.c lua/lgc.c lua/linit.c lua/liolib.c lua/llex.c lua/llimits.h lua/lmathlib.c lua/lmem.c lua/loadlib.c lua/lobject.c lua/lopcodes.c lua/loslib.c lua/lparser.c lua/lstate.c lua/lstring.c lua/lstrlib.c lua/ltable.c lua/ltm.c lua/lundump.c lua/lvm.c lua/lzio.c)

# Add the source in project root directory
aux_source_directory(. DIRSRCS)
# Add header file include directories
include_directories(./ ./lua)

# Add block directories
#add_subdirectory(hello)

# Target
add_executable(cpplua ${DIRSRCS})
target_link_libraries(cpplua lua)