set(RISKEMUGATEWAY_SRC
	"Gateway.h" "Gateway.cpp"
	"AuthServer.h" "AuthServer.cpp"
	"LauncherServer.h" "LauncherServer.cpp")

add_executable(RiskEmuGateway ${RISKEMUGATEWAY_SRC})
target_link_libraries(RiskEmuGateway RiskEmuLibrary)