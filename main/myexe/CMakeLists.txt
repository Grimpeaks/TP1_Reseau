cmake_minimum_required(VERSION 3.12)

set(SOURCE_FOLDER ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INCLUDE_FOLDER ${CMAKE_CURRENT_SOURCE_DIR}/include)

#Include the whole include directory for this directory
include_directories(${INCLUDE_FOLDER})

#Source files
set(SOURCES ${SOURCE_FOLDER}/main.cpp)

#Headers files (for IntelliSense)
set(HEADERS ${INCLUDE_FOLDER}/main.hpp)

#Main exec
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})