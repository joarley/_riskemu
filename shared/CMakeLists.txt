set(Boost_USE_STATIC_LIBS ON)
find_package(Boost)
include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIRS})

set(RISKEMULIBRARY_HDR "./network/Client.h" "./network/Server.h" "./stdtypes.h")
set(RISKEMULIBRARY_SRC "./network/Client.cpp" "./network/Server.cpp")


add_library(RiskEmuLibrary ${RISKEMULIBRARY_SRC} ${RISKEMULIBRARY_HDR})
target_link_libraries(RiskEmuLibrary ${Boost_LIBRARIES})
