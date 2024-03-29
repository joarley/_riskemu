cmake_minimum_required(VERSION 2.6)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules")

project(RiskEmu)

set(Boost_USE_STATIC_LIBS ON)
find_package(Boost COMPONENTS thread system date_time REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

find_package(Lua REQUIRED)
include_directories(${LUA_INCLUDE_DIR})

include_directories(./shared)

if(WIN32)
	add_definitions(-D_WIN32_WINNT=0x0600)
endif()
if(MSVC)
	add_definitions(-DBOOST_ALL_NO_LIB)
endif()

add_subdirectory(./shared)
add_subdirectory(./gateway)


option(COMPILER_PROTOCOL_ANALYZER "Compiler Protocol Analyzer Server" ON)
if(COMPILER_PROTOCOL_ANALYZER)
    add_subdirectory("./tools/protocol analyzer/server")
endif()
