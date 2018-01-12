cmake_minimum_required(VERSION 3.6)
project(PcapReleay)
set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES  main.cpp src/LinuxEncalsulation.cpp include/LinuxEncalsulation.h src/PCAP.cpp include/PCAP.h
        src/PCAPHistory.cpp include/PCAPHistory.h src/IO/Writer/PCAPWriter.cpp include/IO/Writer/PCAPWriter.h src/IO/Reader/Pipe/PCAPPipeReader.cpp
        include/IO/Reader/PCAPPipeReader.h include/IO/Reader/PCAPSSHReader.h src/IO/Reader/SSH/PCAPSSHReader.cpp src/IO/Reader/BasePCAPReader.cpp
        include/IO/Reader/BasePCAPReader.h)



include_directories(lib)
include_directories(include)

link_directories(${CMAKE_SOURCE_DIR}/lib)
include_directories(${GTEST_INCLUDE_DIRS})


add_executable(PcapReleay ${SOURCE_FILES} )


find_package(GTest REQUIRED)
find_library(lib PipeRelay.a)
target_link_libraries(PcapReleay PipeRelay.a pthread)



set(SOURCE_FILESTest src/LinuxEncalsulation.cpp include/LinuxEncalsulation.h src/PCAP.cpp include/PCAP.h
        src/PCAPHistory.cpp include/PCAPHistory.h src/IO/Writer/PCAPWriter.cpp include/IO/Writer/PCAPWriter.h src/IO/Reader/Pipe/PCAPPipeReader.cpp
        include/IO/Reader/PCAPPipeReader.h include/IO/Reader/PCAPSSHReader.h src/IO/Reader/SSH/PCAPSSHReader.cpp src/IO/Reader/BasePCAPReader.cpp
        include/IO/Reader/BasePCAPReader.h Test/integration/integrationTest.cpp Test/pcap/historyTest.cpp Test/init.cpp Test/pcap/pcapTest.cpp Test/sshConfig/sshConfigTest.cpp)



add_executable(PipeRelayTest ${SOURCE_FILESTest} )

target_link_libraries(PipeRelayTest ${GTEST_LIBRARIES}  pthread PipeRelay.a)
