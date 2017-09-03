cmake_minimum_required(VERSION 3.6)
project(PcapReleay)
set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES  main.cpp PipeRelay/PipeRelay/Writer/PipeWriter.cpp PipeRelay/PipeRelay/Writer/PipeWriter.h PipeRelay/PipeRelay/Reader/PipeReader.cpp
        PipeRelay/PipeRelay/Reader/PipeReader.h Loging/Log.h Loging/Log.cpp PipeRelay/PipeRelay/PipeHandler.h PipeRelay/PipeRelay/Pipehandler.cpp
        PipeRelay/PipeRelay/ssh/ReadConfig.h PipeRelay/PipeRelay/ssh/Ssh.cpp PipeRelay/PipeRelay/ssh/Ssh.h PipeRelay/PipeRelay/ssh/ReadConfig.cpp
        PipeRelay/PipeRelay/BaseReader.cpp PipeRelay/PipeRelay/BaseReader.h PCAP/LinuxEncalsulation.cpp PCAP/LinuxEncalsulation.h PCAP/PCAP.cpp PCAP/PCAP.h
        PCAP/PCAPHistory.cpp PCAP/PCAPHistory.h PCAP/IO/Writer/PCAPWriter.cpp PCAP/IO/Writer/PCAPWriter.h PCAP/IO/Reader/Pipe/PCAPPipeReader.cpp
        PCAP/IO/Reader/Pipe/PCAPPipeReader.h PCAP/IO/Reader/SSH/PCAPSSHReader.h PCAP/IO/Reader/SSH/PCAPSSHReader.cpp PCAP/IO/Reader/BasePCAPReader.cpp
        PCAP/IO/Reader/BasePCAPReader.h )

add_executable(PcapReleay ${SOURCE_FILES} )

target_link_libraries(PcapReleay  pthread)





set(SOURCE_FILESTest   PipeRelay/PipeRelay/Writer/PipeWriter.cpp PipeRelay/PipeRelay/Writer/PipeWriter.h PipeRelay/PipeRelay/Reader/PipeReader.cpp
        PipeRelay/PipeRelay/Reader/PipeReader.h Loging/Log.h Loging/Log.cpp PipeRelay/PipeRelay/PipeHandler.h PipeRelay/PipeRelay/Pipehandler.cpp
        PipeRelay/PipeRelay/ssh/ReadConfig.h PipeRelay/PipeRelay/ssh/Ssh.cpp PipeRelay/PipeRelay/ssh/Ssh.h PipeRelay/PipeRelay/ssh/ReadConfig.cpp
        PipeRelay/PipeRelay/BaseReader.cpp PipeRelay/PipeRelay/BaseReader.h PCAP/LinuxEncalsulation.cpp PCAP/LinuxEncalsulation.h PCAP/PCAP.cpp PCAP/PCAP.h
        PCAP/PCAPHistory.cpp PCAP/PCAPHistory.h PCAP/IO/Writer/PCAPWriter.cpp PCAP/IO/Writer/PCAPWriter.h PCAP/IO/Reader/Pipe/PCAPPipeReader.cpp
        PCAP/IO/Reader/Pipe/PCAPPipeReader.h PCAP/IO/Reader/SSH/PCAPSSHReader.h PCAP/IO/Reader/SSH/PCAPSSHReader.cpp PCAP/IO/Reader/BasePCAPReader.cpp
        PCAP/IO/Reader/BasePCAPReader.h Test/integration/integrationTest.cpp Test/historyTest.cpp Test/init.cpp Test/pcapTest.cpp)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
add_executable(PipeRelayTest ${SOURCE_FILESTest} )
target_link_libraries(PipeRelayTest ${GTEST_LIBRARIES}  pthread)