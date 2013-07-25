find_package(Boost)
include_directories(${Boost_INCLUDE_DIR})

set(RISKEMULIBRARY_HDR "./network/windows/Client.h" "./network/windows/Server.h"
                       "./network/Client.h" "./network/Server.h" "./stdtypes.h")
set(RISKEMULIBRARY_SRC "./network/windows/Client.cpp" "./network/windows/Server.cpp")


add_library(RiskEmuLibrary SHARED ${RISKEMULIBRARY_SRC} ${RISKEMULIBRARY_HDR})
