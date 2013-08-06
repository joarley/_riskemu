set(RISKEMUGATEWAY_SRC
	"Gateway.h" "Gateway.cpp")

add_executable(RiskEmuGateway ${RISKEMUGATEWAY_SRC})
target_link_libraries(RiskEmuGateway RiskEmuLibrary)