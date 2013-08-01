cmake_minimum_required(VERSION 2.6)

project(RiskEmu)

set(Boost_USE_STATIC_LIBS ON)
find_package(Boost COMPONENTS thread system REQUIRED)

include_directories(./shared)
include_directories(${Boost_INCLUDE_DIR})

link_directories(${Boost_LIBRARY_DIRS})

if(WIN32)
	add_definitions(-D_WIN32_WINNT=0x0600)
endif()
if(MSVC)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

add_subdirectory(./shared)
add_subdirectory(./gateway)