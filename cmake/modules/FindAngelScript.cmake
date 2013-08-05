#  AngelScript_FOUND - system has Magick++
#  AngelScript_INCLUDE_DIRS - the Magick++ include directories
#  AngelScript_LIBRARIES - link these to use Magick++

find_path(AngelScript_INCLUDE_DIR
  NAMES angelscript.h
  PATHS "$ENV{ANGELSCRIPT_ROOT}/include"
)

find_library(AngelScript_LIBRARY
  NAMES angelscript
  PATHS "$ENV{ANGELSCRIPT_ROOT}/lib"
)

find_library(AngelScript_LIBRARY_DEBUG
  NAMES angelscriptd
  PATHS "$ENV{ANGELSCRIPT_ROOT}/lib"
)

set(AngelScript_LIBRARIES optimized ${AngelScript_LIBRARY} debug ${AngelScript_LIBRARY_DEBUG})
set(AngelScript_INCLUDE_DIRS ${AngelScript_INCLUDE_DIR})