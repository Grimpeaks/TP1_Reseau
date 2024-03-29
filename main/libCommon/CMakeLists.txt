cmake_minimum_required(VERSION 3.12)

set(SOURCE_FOLDER ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INCLUDE_FOLDER ${CMAKE_CURRENT_SOURCE_DIR}/include)

#Include the whole include directory for this directory
include_directories(${INCLUDE_FOLDER})

#Source files
set(SOURCES ${SOURCE_FOLDER}/network.cpp)
set(SOURCES ${SOURCE_FOLDER}/Message.cpp)

#Headers files (for IntelliSense)
set(HEADERS ${INCLUDE_FOLDER}/network.hpp)
set(HEADERS ${INCLUDE_FOLDER}/Message.hpp)

#Windows source and headers
if(WIN32)
	set(SOURCES ${SOURCES} ${SOURCE_FOLDER}/winnetworkconfig.cpp)
	set(HEADERS ${HEADERS} ${INCLUDE_FOLDER}/winnetworkconfig.hpp)
endif()

#Main lib
add_library(common_lib ${SOURCES} ${HEADERS})
target_include_directories(common_lib SYSTEM INTERFACE ${INCLUDE_FOLDER})
