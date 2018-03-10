cmake_minimum_required(VERSION 3.5)
project(PcapStreamMerge)
set(CMAKE_CXX_STANDARD 11)

include_directories(lib)
include_directories(include)
include_directories(PipeStreamMerge/include)
include_directories(PipeStreamMerge)



set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR})

set(PcapMergeFiles  ${PipeStreamMergeSOURCEFILES} src/LinuxEncalsulation.cpp include/LinuxEncalsulation.h src/PCAP.cpp include/PCAP.h
        src/PCAPHistory.cpp include/PCAPHistory.h src/IO/Writer/PCAPWriter.cpp include/IO/Writer/PCAPWriter.h src/IO/Reader/Pipe/PCAPPipeReader.cpp
        include/IO/Reader/PCAPPipeReader.h include/IO/Reader/PCAPSSHReader.h src/IO/Reader/SSH/PCAPSSHReader.cpp src/IO/Reader/BasePCAPReader.cpp
        include/IO/Reader/BasePCAPReader.h)


set(PcapMergeFilesTest Test/integration/integrationTest.cpp Test/pcap/historyTest.cpp Test/init.cpp Test/pcap/pcapTest.cpp
        Test/sshConfig/sshConfigTest.cpp Test/gtest/gtest.h Test/gtest/gtest-all.cc)


include(ExternalProject)
externalProject_add(
        logging
        GIT_REPOSITORY https://github.com/diridari/SimpleLogging.git
        CONFIGURE_COMMAND   cmake CMakeLists.txt
        BUILD_IN_SOURCE ON
        INSTALL_COMMAND make SimpleLoggingLib   # make target
)
ExternalProject_Get_Property(logging SOURCE_DIR)
ExternalProject_Get_Property(logging BINARY_DIR)
include_directories(${SOURCE_DIR}/include) # include simpleLog/inlude
LINK_DIRECTORIES(${BINARY_DIR}) ## link git build lib



include_directories(Test/gtest)
add_subdirectory(PipeStreamMerge)
enable_testing()

add_executable(PcapStreamMerge ${PcapMergeFiles} ${SOURCE_DIR}  main.cpp)
add_executable(PcapStreamMergeTest ${PcapMergeFilesTest} ${SOURCE_DIR}  ${PcapMergeFiles} ${PipeStreamMerge_SOURCE_FILES})


add_test(UnitTests PcapStreamMergeTest)

target_link_libraries(PcapStreamMerge  pthread PipeRelayLib )
target_link_libraries(PcapStreamMergeTest ${GTEST_LIBRARIES}  pthread PipeRelayLib )

ADD_DEPENDENCIES(PcapStreamMerge logging )
ADD_DEPENDENCIES(PcapStreamMergeTest logging)
