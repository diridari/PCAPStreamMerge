//
// Created by basti on 3/15/17.
//

#ifndef PIPERELAYV2_MAIN_H
#define PIPERELAYV2_MAIN_H

#include <iostream>
#include <string.h>
#include <thread>
#include <vector>
#include "PipeRelay/Writer/PipeWriter.h"
#include "PipeRelay/ssh/Ssh.h"


using namespace std;

/**
 * Struct witch is describing the inpipe *
 */
struct threadStartPipe{
    string *name;
    string *location;
    PipeWriter *outWriter;
    bool message;
    bool log;
    threadStartPipe(string *location, PipeWriter *outPipe, string *name = NULL, bool message = true, bool log = false)
            :name(name),location(location),outWriter(outPipe),log(log), message(message){};

};

string location = "/tmp/pipeRelay";     // default location
string configFile = "";                 // location of the config File
bool doSSH = false;                     // shall the program connect via ssh to the clients (config file)
bool doPipe = false;                    // Shall the program create named Pipes
int numberOfPipes = 0;                  // number of named Pipes
vector<thread*> *l = new vector<thread*>(); // list of all Threads
PipeWriter *wr;                         // writing end of the program

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
void startSSHReader(ssh *s);

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
