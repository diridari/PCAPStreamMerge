//
// Created by basto on 7/3/17.
//

#ifndef PIPERELAYV2_BASEPIPEREADER_H
#define PIPERELAYV2_BASEPIPEREADER_H


#include "../Writer/PCAPWriter.h"

class BasePCAPReader {
    bool isOpen = false;
    BaseReader * reader;
    string name;
    PCAPWriter *outWriter;
    bool hasOverhead;
protected:
    /**
      * read an hole pcap and send it to the writer
      * @return success
      */
    bool nextPCAP(); // and write it to out

    /**
     * reads until it found the first "\n" :: new line
     * is needet if the pipe does have overhead like tcpdump content

     */
    void cutOverhead();

public:
    BasePCAPReader(BaseReader *reader, PCAPWriter *out,string name = "nunnamed writer",bool hasOverhead = true):
            reader(reader),name(name), hasOverhead(hasOverhead), outWriter(out){

    }
    /**
        * open thePipe
        * blocks until the pipe is opend
        * @return pipe is opened
        */
    bool open(){
        isOpen =  reader->open();
        return isOpen;
    }

    /**
     * runs until the pipe is closed
     * each loop step reads one pcap and send it to the writer
     *
     */
    void run();

    /**
    * close the named pipe and delete the file
    */
    void close(){
        reader->close();
    }
};


#endif //PIPERELAYV2_BASEPIPEREADER_H
