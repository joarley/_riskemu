set(RISKEMULIBRARY_SRC 
"./stdtypes.h"
"./network/Client.h" "./network/Client.cpp" "./network/Server.h" "./network/Server.cpp"
"./packet/PacketBase.h" "./packet/PacketBase.cpp"
"./minilzo/MiniLZO.h" "./minilzo/MiniLZO.cpp"
"./crc/CRC32.h" "./crc/CRC32.tab"
)


add_library(RiskEmuLibrary ${RISKEMULIBRARY_SRC})
target_link_libraries(RiskEmuLibrary ${Boost_LIBRARIES})
