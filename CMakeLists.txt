cmake_minimum_required(VERSION 3.5)
project(PcapStreamMerge)
set(CMAKE_CXX_STANDARD 11)

find_package(GTest REQUIRED)

set(SOURCE_FILES  src/LinuxEncalsulation.cpp include/LinuxEncalsulation.h src/PCAP.cpp include/PCAP.h
        src/PCAPHistory.cpp include/PCAPHistory.h src/IO/Writer/PCAPWriter.cpp include/IO/Writer/PCAPWriter.h src/IO/Reader/Pipe/PCAPPipeReader.cpp
        include/IO/Reader/PCAPPipeReader.h include/IO/Reader/PCAPSSHReader.h src/IO/Reader/SSH/PCAPSSHReader.cpp src/IO/Reader/BasePCAPReader.cpp
        include/IO/Reader/BasePCAPReader.h)

## TODO FIX this
set(SOURCE_FILES_Pipe    PipeStreamMerge/src/Writer/PipeWriter.cpp PipeStreamMerge/include/PipeWriter.h PipeStreamMerge/src/Reader/PipeReader.cpp
        PipeStreamMerge/include/PipeReader.h PipeStreamMerge/include/Log.h PipeStreamMerge/src/Loging/Log.cpp PipeStreamMerge/include/PipeHandler.h PipeStreamMerge/src/Pipehandler.cpp
        PipeStreamMerge/include/ReadConfig.h PipeStreamMerge/src/ssh/Ssh.cpp PipeStreamMerge/include/Ssh.h PipeStreamMerge/src/ssh/ReadConfig.cpp
        PipeStreamMerge/src/BaseReader.cpp PipeStreamMerge/include/BaseReader.h)

set(SOURCE_FILESTest Test/integration/integrationTest.cpp Test/pcap/historyTest.cpp Test/init.cpp Test/pcap/pcapTest.cpp Test/sshConfig/sshConfigTest.cpp)




include_directories(lib)
include_directories(include)
include_directories(PipeStreamMerge/include)

link_directories(${CMAKE_SOURCE_DIR}/lib)
include_directories(${GTEST_INCLUDE_DIRS})

enable_testing()

add_executable(PcapStreamMerge ${SOURCE_FILES} ${SOURCE_FILES_Pipe}  main.cpp)
add_executable(PcapStreamMergeTest ${SOURCE_FILESTest}  ${SOURCE_FILES} ${SOURCE_FILES_Pipe})

add_test(UnitTests PcapStreamMergeTest)

target_link_libraries(PcapStreamMerge  pthread)
target_link_libraries(PcapStreamMergeTest ${GTEST_LIBRARIES}  pthread )
