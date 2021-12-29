cmake_minimum_required (VERSION 2.6)
project (game)


set (SOURCE_DIR src)
set (SRC_LIST
	main.cpp
)

set (LIB_INCLUDE_DIRS
	#lib/SDL2_mixer-2.0.4/include/ 
	/lib/SDL2-2.0.12/include/
)
set (LIB_SOURCE_DIRS 
	#lib/SDL2_mixer-2.0.4/lib/
	/lib/SDL2-2.0.12/lib/
)

include_directories("${SOURCE_DIR}/")
#add_library(SDL_mixer SHARED lib/SDL2_mixer-2.0.4/lib/x64/SDL2_mixer.dll)

#find_package(SDL_mixer REQUIRED)
find_package(SDL2 REQUIRED)
include_directories(${SDL_MIXER_INCLUDE_DIRS})
include_directories(${SDL2_INCLUDE_DIRS})

set (LIB_DIR
	lib
)



set (ADDITIONAL_SRCS
	util.cpp
	Game.cpp
	Graphics.cpp
	Image.cpp
	Input.cpp
	Parser.cpp
	GameObject.cpp
)

foreach (source ${ADDITIONAL_SRCS})
	set (SRC_LIST ${SRC_LIST} ${SOURCE_DIR}/${source})
endforeach()

add_executable(game ${SRC_LIST})
target_link_libraries(game ${SDL2_LIBRARIES})

set (CMAKE_CXX_COMPILER g++)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++2a")

