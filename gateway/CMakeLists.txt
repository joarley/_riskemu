set(RISKEMUGATEWAY_HDR "")
set(RISKEMUGATEWAY_SRC "main.cpp")


add_executable(RiskEmuGateway ${RISKEMUGATEWAY_SRC} ${RISKEMUGATEWAY_HDR})
target_link_libraries(RiskEmuGateway RiskEmuLibrary)