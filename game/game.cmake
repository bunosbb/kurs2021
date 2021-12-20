set ("${PROJECT}_BINARY_DIR"  bin)
set ("${PROJECT}_LIB_DIR" lib)
set ("$[PROJECT}_SRC_DIR" src)

set (CMAKE_INCLUDE_PATH ${${PROJECT}_SOURCE_DIR})
set (CMAKE_LIBRARY_PATH ${${PROJECT}_LIB_DIR})
set (EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/${${PROJECT}_BINARY_DIR})
#set (CMAKE_VERBOSE_MAKEFILE ON)


set (ERR_NO_UNIX "Cannot build on non Unix systems")

if (CMAKE_COMPILER_IS_GNUCXX)
    set (MY_CXX_FLAGS  "-Wall -std=c++0x -fmessage-length=0 -v -L/git/lol/game/lib -L/game/lib")
    set (CMAKE_CXX_FLAGS "-O0 ${MY_CXX_FLAGS}")
else ()
    message (FATAL_ERROR ${ERR_NO_UNIX})
endif ()