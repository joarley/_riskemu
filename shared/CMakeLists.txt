option(MINILZO_USE_ORIGINAL_C_LIBRARY "Use Oberhumer Mini LZO" OFF)

set(RISKEMULIBRARY_SRC 
	"./stdtypes.h"
	"./network/Client.h" "./network/Client.cpp" "./network/Server.h" "./network/Server.cpp"
	"./packet/PacketBase.h" "./packet/PacketBase.cpp"
	"./minilzo/MiniLZO.h" "./minilzo/MiniLZO.cpp"
	"./crc/CRC32.h" "./crc/CRC32.tab"
)

if(MINILZO_USE_ORIGINAL_C_LIBRARY)
	add_definitions(-DMINILZO_USE_ORIGINAL_C_LIBRARY)

	set(RISKEMULIBRARY_SRC ${RISKEMULIBRARY_SRC}
		"./minilzo/oberhumer/lzoconf.h" "./minilzo/oberhumer/lzodefs.h"
		"./minilzo/oberhumer/minilzo.c" "./minilzo/oberhumer/minilzo.h"
	)
endif()


add_library(RiskEmuLibrary ${RISKEMULIBRARY_SRC})
target_link_libraries(RiskEmuLibrary ${Boost_LIBRARIES})
