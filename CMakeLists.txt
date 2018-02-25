cmake_minimum_required(VERSION 3.6)
project(PcapReleay)
set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES  src/LinuxEncalsulation.cpp include/LinuxEncalsulation.h src/PCAP.cpp include/PCAP.h
        src/PCAPHistory.cpp include/PCAPHistory.h src/IO/Writer/PCAPWriter.cpp include/IO/Writer/PCAPWriter.h src/IO/Reader/Pipe/PCAPPipeReader.cpp
        include/IO/Reader/PCAPPipeReader.h include/IO/Reader/PCAPSSHReader.h src/IO/Reader/SSH/PCAPSSHReader.cpp src/IO/Reader/BasePCAPReader.cpp
        include/IO/Reader/BasePCAPReader.h)

## TODO FIX this
set(SOURCE_FILES_PCAP    PipeRelay/src/Writer/PipeWriter.cpp PipeRelay/include/PipeWriter.h PipeRelay/src/Reader/PipeReader.cpp
        PipeRelay/include/PipeReader.h PipeRelay/include/Log.h PipeRelay/src/Loging/Log.cpp PipeRelay/include/PipeHandler.h PipeRelay/src/Pipehandler.cpp
        PipeRelay/include/ReadConfig.h PipeRelay/src/ssh/Ssh.cpp PipeRelay/include/Ssh.h PipeRelay/src/ssh/ReadConfig.cpp
        PipeRelay/src/BaseReader.cpp PipeRelay/include/BaseReader.h)

set(SOURCE_FILESTest Test/integration/integrationTest.cpp Test/pcap/historyTest.cpp Test/init.cpp Test/pcap/pcapTest.cpp Test/sshConfig/sshConfigTest.cpp)


include_directories(lib)
include_directories(include)
include_directories(PipeRelay/include)

link_directories(${CMAKE_SOURCE_DIR}/lib)
include_directories(${GTEST_INCLUDE_DIRS})


add_executable(PcapReleay ${SOURCE_FILES} ${SOURCE_FILES_PCAP}  main.cpp)


find_package(GTest REQUIRED)
target_link_libraries(PcapReleay  pthread)






add_executable(PipeRelayTest ${SOURCE_FILESTest}  ${SOURCE_FILES})

target_link_libraries(PipeRelayTest ${GTEST_LIBRARIES}  pthread PipeRelay.a)
