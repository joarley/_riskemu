cmake_minimum_required(VERSION 2.6)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

project(RiskEmu)

set(Boost_USE_STATIC_LIBS ON)

find_package(Boost COMPONENTS thread system date_time REQUIRED)
include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIRS})

find_package(AngelScript REQUIRED)
link_directories(${AngelScript_LIBRARY_DIRS})
include_directories(${AngelScript_INCLUDE_DIR})

include_directories(./shared)

if(WIN32)
	add_definitions(-D_WIN32_WINNT=0x0600)
endif()
if(MSVC)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS -DBOOST_ALL_NO_LIB)
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
	set(CMAKE_CSS_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")	
endif()

add_subdirectory(./shared)
add_subdirectory(./gateway)
