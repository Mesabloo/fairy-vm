cmake_minimum_required(VERSION 3.8)
project(VM CXX)

include_directories(${FairyVM_SOURCE_DIR}/include ${FairyVM_SOURCE_DIR}/ext/argh ${FairyVM_SOURCE_DIR}/ext/termcolor/include)
file(GLOB_RECURSE SOURCE_FILES ${VM_SOURCE_DIR}/*.cpp)

message(STATUS "Found compilable files: ${SOURCE_FILES}")

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

if(NOT MSVC)
    target_link_libraries(${PROJECT_NAME} stdc++fs)
endif()
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

option(CMAKE_BUILD_TYPE "DEBUG or RELEASE" "DEBUG")

set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "fvm")
if (MSVC)
	if("${CMAKE_BUILD_TYPE}" STREQUAL "RELEASE")
    	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /D NOMINMAX /D NDEBUG")
    else()
    	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W3 /WX- /D NOMINMAX")
    endif()
endif (MSVC)

if (CMAKE_COMPILER_IS_GNUCC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -L/usr/local/lib -Wl,-R/usr/local/lib")
    if("${CMAKE_BUILD_TYPE}" STREQUAL "RELEASE")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -march=native -DNDEBUG")
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g3 -Wall -Wextra -pedantic -W -fsanitize=address,undefined")
    endif()
endif()

message(STATUS "Compiler flags: ${CMAKE_CXX_FLAGS}")