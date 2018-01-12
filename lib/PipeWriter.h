//
// Created by Sebastian Balz  on 1/13/17.
//


#ifndef THREADPIPE_WRITER_H
#define THREADPIPE_WRITER_H



#include "PipeHandler.h"
#include <mutex>
#include <fstream>
#include "BaseReader.h"


#include <sys/stat.h>

using namespace std;

/**
 * writer class
 *  the writer obj shall be unique
 *  the reader objects sends there data to this obj.
 *  this class writes the data to an named pipe witch can be read ab an application like wireshark
 *
 */
class PipeWriter : public PipeHandler{


    ofstream *fwriter = NULL;                       // out Pipe
    char loglocation[20] = "/tmp/log";
    ofstream *loger = new ofstream(loglocation);    //  filteWriter   writes pcap into an external file if writeLog == true
                                                    // location of the Pipe
    std::mutex locker;                              // Mutex ist used to detect the first pipe witch get opend


    bool writeLog;

    bool isOpen = false;
public:
   /**
    * create an writer object
    * usually thera more plenty reader and on writer each writer has one pipe
    * the writer object has the be opened manually. This is going to blocks until an applications opens the other end of the pipe.
    * @param location           the pipes location
    * @param name               the name of the Obj   the name is printed in message and log
    * @param avoidDuplicates    enables an detection if the same(decided by hash) pcap-package has been received in the last <historyTime> ms
    * @param historyTime        how long does the program store data to void duplicate pcaps in ms  default is 500 ms
    * @param message            shall the program write messages to the cli
    * @param log                shall the program write log the the cli and write additional the pcap to an external file
    */
    PipeWriter(string *location = new string("/tmp/pipeRelayOut"),string *name = new string("unnamed writer"), bool log = false):
                PipeHandler(*location,*name), writeLog(log){
    };


    /**
     * open the pipe
     * blocks until an application opens the other end of the pipe
     * @return open success
     */
    bool open();


    /**
     * write an array to the out pipe
     * this is used eg if an thread write the first pcap-File-Header
     * @param buffer  array to write to out pipe
     * @param size of array
     */
    void write(char * buffer, int size);


    /**
     * close the named pipe and delete the pipe-file
     */
    void close();
};


#endif //THREADPIPE_WRITER_H
