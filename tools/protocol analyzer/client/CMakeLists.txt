find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
include_directories(${GTK3_INCLUDE_DIRS})
link_directories(${GTK3_LIBRARY_DIRS})
add_definitions(${GTK3_CFLAGS_OTHER})

set(PKTANALYZER_SRC "main.cpp"
    "protocoleditor/EmiterPacketTree.h" "protocoleditor/EmiterPacketTree.cpp"
   "protocoleditor/PacketContentTree.h" "protocoleditor/PacketContentTree.cpp")


add_executable(protocolanalyzer.client ${PKTANALYZER_SRC})

target_link_libraries(protocolanalyzer.client ${GTK3_LIBRARIES})
