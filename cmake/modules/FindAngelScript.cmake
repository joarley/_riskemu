#  AngelScript_FOUND - system has AngelScript
#  AngelScript_INCLUDE_DIRS - the AngelScript include directories
#  AngelScript_LIBRARIES - link these to use AngelScript

if(${ANGELSCRIPT_ROOT} STREQUAL "")
  if(NOT $ENV{ANGELSCRIPT_ROOT} STREQUAL "")
    set(ANGELSCRIPT_ROOT $ENV{BOOST_ROOT})
  endif()
endif()

find_path(AngelScript_INCLUDE_DIR
  NAMES angelscript.h
  PATHS "${ANGELSCRIPT_ROOT}/include"
)

find_library(AngelScript_LIBRARY
  NAMES Angelscript
  PATHS "${ANGELSCRIPT_ROOT}/lib"
)

find_library(AngelScript_LIBRARY_DEBUG
  NAMES Angelscriptd
  PATHS "${ANGELSCRIPT_ROOT}/lib"
)

set(AngelScript_LIBRARIES optimized ${AngelScript_LIBRARY} debug ${AngelScript_LIBRARY_DEBUG})
set(AngelScript_INCLUDE_DIRS ${AngelScript_INCLUDE_DIR})
