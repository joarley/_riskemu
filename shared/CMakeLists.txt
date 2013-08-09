option(MINILZO_USE_ORIGINAL_C_LIBRARY "Use Oberhumer Mini LZO" OFF)

set(RISKEMULIBRARY_SRC 
	"./stdtypes.h" "./Singleton.h" "Splash.h" "main.h"
	"./network/Client.h" "./network/Client.cpp" "./network/Server.h" "./network/Server.cpp"
	"./network/ServiceContainer.h"
	"./packet/PacketBase.h" "./Buffer.h" "./Packable.h"
	"./minilzo/MiniLZO.h" "./minilzo/MiniLZO.cpp"
	"./crc/CRC32.h" "./crc/CRC32.tab"
	"./crypt/CryptEngine.h" "./crypt/CryptEngine.cpp" "./crypt/CryptEngine.tab"
	"./crypt/seed.tab" "./crypt/SeedCipher.cpp" "./crypt/SeedCipher.h"
	"./Utils.h"
	"./packet/PktPing.h" "./packet/PktAuthServer.h" "./packet/PktAuthServerAck.h"
	"./script/ScriptContext.h" "./script/scriptstdstring/scriptstdstring.cpp" 
	"./script/scriptstdstring/scriptstdstring.h" "./script/scriptstdstring/scriptstdstring_utils.cpp"
	"./script/scriptarray/scriptarray.cpp" "./script/scriptarray/scriptarray.h"
	"./log/Logger.h" "./log/Logger.cpp")

if(MINILZO_USE_ORIGINAL_C_LIBRARY)
	add_definitions(-DMINILZO_USE_ORIGINAL_C_LIBRARY)
	set(RISKEMULIBRARY_SRC ${RISKEMULIBRARY_SRC}
		"./minilzo/oberhumer/lzoconf.h" "./minilzo/oberhumer/lzodefs.h"
		"./minilzo/oberhumer/minilzo.c" "./minilzo/oberhumer/minilzo.h")
endif()


add_library(RiskEmuLibrary ${RISKEMULIBRARY_SRC})
target_link_libraries(RiskEmuLibrary ${Boost_LIBRARIES} ${AngelScript_LIBRARIES})