//
// Created by basti on 3/15/17.
//

#ifndef PIPERELAYV2_MAIN_H
#define PIPERELAYV2_MAIN_H

#include <iostream>
#include <string.h>
#include <thread>

#include "PCAP/LinuxEncalsulation.h"
#include "PCAP/IO/Writer/PCAPWriter.h"
#include "PipeRelay/PipeRelay/ssh/Ssh.h"
#include "PCAP/IO/Reader/SSH/PCAPSSHReader.h"


using namespace std;
/**
 * Struct witch is describing the inpipe *
 */
struct threadStartPipe{
    string *name;
    string *location;
    PCAPWriter *outWriter;
    bool message;
    bool log;
    threadStartPipe(string *location, PCAPWriter *outPipe, string *name = NULL, bool message = true, bool log = false)
            :name(name),location(location),outWriter(outPipe),log(log), message(message){};

};

string location = "/tmp/pipeRelay";     // default location
string configFile = "";                 // location of the config File
string channel = "26";                  // default channel
bool doSSH = false;                     // shall the program connect via ssh to the clients (config file)
bool remoteSetup = true;                // shall the program trigger the remoteSetup
bool doPipe = false;                    // Shall the program create named Pipes

uint historyTime = 0;                    // history time
int numberOfPipes = 0;                  // number of named Pipes
vector<thread*> *l = new vector<thread*>(); // list of all Threads

PCAPWriter *wr;                         // writing end of the program PCAP
PipeWriter *pw;                          // pipe Writer
/**
 * this function shall run in an thread and describe the life of the thread
 * the function opens the thread
 * and starts the run
 * @param w pointer to the struct witch is describing the  inpipe
 */
void startPipeReader(threadStartPipe *w);

/**
 * this function shall run in an single thread and describe the life of an ssh connection
 * this function shall be the first function after thread start
 * @param s ssh new object  the function calls open and run
 */
void startSSHReader(PCAPSSHReader *s);

/**
 * the program parameter is ssh and the program shall run in ssh mode
 * @param s location to the config file
 */
void handleSSH(string s);

/**
 * the program shall run in named-pipe mode
 * @param numberOfThreads number of pipes witch the program cread
 */
void handleNamedPipe(int numberOfThreads);

/**
 *
 */
void configuration(int numberOfArg, char *argv[]);
#endif //PIPERELAYV2_MAIN_H