//
// Created by Sebastian Balz on 1/28/17.
//

#ifndef THREADPIPE_PIPEHANDLER_H
#define THREADPIPE_PIPEHANDLER_H

#include <string>
#include <iostream>

#include <sys/stat.h>
#include <string>
#include <cstring>
#include "Log.h"
using namespace std;
class PipeHandler{



protected:

    /**
     * name
     * if the program print an information to the cli
     * the name determ witch thread sends this information
     */
    string name;
    /**
     * location of the named pipe
     */
    string location;

    /**
     * to check if the pipe is open
     */
    bool isOpen = false;
    PipeHandler(string location, string name):
            location(location), name(name){

    };

    ~PipeHandler(){
        char *tmp = new char(location.size());
        strcpy(tmp,location.c_str());
        remove(tmp);
    }

   /**
   * open thePipe
   * blocks until the pipe is opend
   * @return pipe is opened
   */
    virtual bool open() = 0;



    /**
     * creat an pipe at location
     * if thre is an other file the old file gets deleted
     * @return char array with the location is needed for opening the pipe with ifstream
     */
    char* createPipe();

    /**
    * close the named pipe and delete the file
    */
    virtual void close() = 0;




};
#endif //THREADPIPE_PIPEHANDLER_H
