cmake_minimum_required(VERSION 3.6)
project(PipeRelayV2)



set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES  main.cpp PipeRelay/Writer/PipeWriter.cpp PipeRelay/Writer/PipeWriter.h PipeRelay/Reader/PipeReader.cpp
        PipeRelay/Reader/PipeReader.h Loging/Log.h Loging/Log.cpp PipeRelay/PipeHandler.h PipeRelay/Pipehandler.cpp
        PipeRelay/ssh/ReadConfig.h PipeRelay/ssh/Ssh.cpp PipeRelay/ssh/Ssh.h PipeRelay/ssh/ReadConfig.cpp
        PipeRelay/BaseReader.cpp PipeRelay/BaseReader.h PCAP/LinuxEncalsulation.cpp PCAP/LinuxEncalsulation.h PCAP/PCAP.cpp PCAP/PCAP.h PCAP/PCAPHistory.cpp PCAP/PCAPHistory.h PCAP/IO/Writer/PCAPWriter.cpp PCAP/IO/Writer/PCAPWriter.h PCAP/IO/Reader/Pipe/PCAPPipeReader.cpp PCAP/IO/Reader/Pipe/PCAPPipeReader.h PCAP/IO/Reader/SSH/PCAPSSHReader.h PCAP/IO/Reader/SSH/PCAPSSHReader.cpp PCAP/IO/Reader/BasePCAPReader.cpp PCAP/IO/Reader/BasePCAPReader.h)



add_executable(PipeRelayV2 ${SOURCE_FILES} )

target_link_libraries(PipeRelayV2  pthread)