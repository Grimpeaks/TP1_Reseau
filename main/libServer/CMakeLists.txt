cmake_minimum_required(VERSION 3.12)

set(SOURCE_FOLDER ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INCLUDE_FOLDER ${CMAKE_CURRENT_SOURCE_DIR}/include)

#Include the whole include directory for this directory
include_directories(${INCLUDE_FOLDER})

#Source files
set(SOURCES ${SOURCE_FOLDER}/server_lib.cpp)

#Headers files (for IntelliSense)
set(HEADERS ${INCLUDE_FOLDER}/server_lib.hpp)

#Main lib
add_library(server_lib ${SOURCES} ${HEADERS})
target_include_directories(server_lib SYSTEM INTERFACE ${INCLUDE_FOLDER})

target_link_libraries(server_lib PUBLIC common_lib)