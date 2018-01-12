cmake_minimum_required(VERSION 3.6)
project(PcapReleay)
set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES  main.cpp  PCAP/LinuxEncalsulation.cpp include/LinuxEncalsulation.h PCAP/PCAP.cpp include/PCAP.h
        PCAP/PCAPHistory.cpp include/PCAPHistory.h PCAP/IO/Writer/PCAPWriter.cpp include/IO/Writer/PCAPWriter.h PCAP/IO/Reader/Pipe/PCAPPipeReader.cpp
        include/IO/Reader/PCAPPipeReader.h include/IO/Reader/PCAPSSHReader.h PCAP/IO/Reader/SSH/PCAPSSHReader.cpp PCAP/IO/Reader/BasePCAPReader.cpp
        include/IO/Reader/BasePCAPReader.h)



include_directories(lib)
include_directories(include)

link_directories(${CMAKE_SOURCE_DIR}/lib)



add_executable(PcapReleay ${SOURCE_FILES} )



find_library(lib PipeRelay.a)
target_link_libraries(PcapReleay PipeRelay.a)
target_link_libraries(PcapReleay  pthread)


#set(SOURCE_FILESTest   PCAP/LinuxEncalsulation.cpp PCAP/LinuxEncalsulation.h PCAP/PCAP.cpp PCAP/PCAP.h
#        PCAP/PCAPHistory.cpp PCAP/PCAPHistory.h PCAP/IO/Writer/PCAPWriter.cpp PCAP/IO/Writer/PCAPWriter.h PCAP/IO/Reader/Pipe/PCAPPipeReader.cpp
#        PCAP/IO/Reader/Pipe/PCAPPipeReader.h PCAP/IO/Reader/SSH/PCAPSSHReader.h PCAP/IO/Reader/SSH/PCAPSSHReader.cpp PCAP/IO/Reader/BasePCAPReader.cpp
#        PCAP/IO/Reader/BasePCAPReader.h Test/integration/integrationTest.cpp Test/pcap/historyTest.cpp Test/init.cpp Test/pcap/pcapTest.cpp Test/sshConfig/sshConfigTest.cpp)

#find_package(GTest REQUIRED)
#include_directories(${GTEST_INCLUDE_DIRS})
#add_executable(PipeRelayTest ${SOURCE_FILESTest} )

#ADD_LIBRARY(pipeRelay STATIC IMPORTED)
#SET_TARGET_PROPERTIES(pipeRelay PROPERTIES IMPORTED_LOCATION lib/libPipeRelay.a)
#TARGET_LINK_LIBRARIES(mytarget A boost_unit_test_framework C)


#target_link_libraries(PipeRelayTest ${GTEST_LIBRARIES}  pthread )