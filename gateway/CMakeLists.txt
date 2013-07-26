set(Boost_USE_STATIC_LIBS ON)
find_package(Boost)
include_directories(${Boost_INCLUDE_DIR})

set(RISKEMUGATEWAY_HDR "")
set(RISKEMUGATEWAY_SRC "main.cpp")


add_executable(RiskEmuGateway ${RISKEMUGATEWAY_SRC} ${RISKEMUGATEWAY_HDR})
target_link_libraries(RiskEmuGateway RiskEmuLibrary)