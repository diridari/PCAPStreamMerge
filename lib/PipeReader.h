//
// Created by Sebastian Balz  on 1/13/17.
//

#ifndef THREADPIPE_READER_H
#define THREADPIPE_READER_H

#include <iostream>
#include <fstream>
#include "PipeWriter.h"
#include <thread>

#include <mutex>

using namespace std;
/*
static bool FIRSTPIPEISOPEN = false;
static std::mutex readerMutex;          // mutex : wird der Reader gerade verwendent
static bool firstPcapSend = false;      // wurde bereits ein PCAP paket gesendet  wird genötigt um den Timstamp zu verifizieren
static char firstTimeStamp[8] = {};     // timestamp des ersten PCAP paketes
*(

/**
 * Reader class
 * there are several reader obj all having the same writer
 * each reader waits until the named pipe gets opend
 * if this reader is the first opend pipe its pcap-file-header gets written to the writer
 * else the pcap-file-header gets cut out
 *
 * the run method return if the pipe is closed
 * usually an reader runs in one thread and the thread is bind at the livetime of its reader
 *
 * example usage
 *          reader *r = new reader(w->location,w->outWriter,w->name,w->message, w->log);   // create an new reader obj
 *           r->open();                                                                    // open the  named-pipe
 *           r->run();                                                                     // start sniffing loop
 */
class PipeReader : public PipeHandler , public BaseReader{
    ifstream * inReader = NULL; // in reader
    PipeWriter *outwriter;          // outWriter
    bool isOpen = false;
    bool writeLog;



public:

    /**
     * Default Constructor
     * create n new Reader obj
     * usually ther are plenty reader on one Writer
     * each reader is running in this one thread
     * the user has to call open() manually  this blocks until the other end of the pipe is open
     * is that happen the user has to call run()
     * this starts an loop witch is reader each pcap and write it to the outWriter
     * if the other end of the is closes the loop ends and run returns
     * @param location  location of the pipe
     * @param that outWriter pipe obj
     * @param name this name is used to identify an thread on messages /logs
     * @param message shall the program print messages  an message is eg. information what the user has to to
     * @param log  shall the program print log
     */
    PipeReader(string *location, string *name = new string("unnamed reader"), bool message = true, bool log = false):
            PipeHandler(*location, *name), writeLog(log) {};

    ~PipeReader(){
        this->close();
    };

    /**
     * open thePipe
     * blocks until the pipe is opend
     * @return pipe is opened
     */
    bool open();

    void read(char * buffer, int size);

    /**
    * close the named pipe and delete the file
    */
    void close();

    bool hasNext(){
        return inReader->peek() >0;
    }

};

#endif