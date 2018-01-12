//
// Created by basto on 7/3/17.
//

#ifndef PIPERELAYV2_PCAPWRITER_H
#define PIPERELAYV2_PCAPWRITER_H


#include <PipeWriter.h>
#include "../../PCAPHistory.h"
#include <mutex>

class PCAPWriter{
    bool isOpen = false;
    PipeWriter *writer;
    bool avoidDuplicates;
    PCAPHistory *history;
    bool hasPCAPHeader = false;
    mutex mutex1;
    string name;
public:

    PCAPWriter(PipeWriter *writer, string name, bool avoidDuplicates = false, uint historyTime = 1000):
            writer(writer), avoidDuplicates(avoidDuplicates), name(name)   {

                history = new PCAPHistory(historyTime);

    }
    ~PCAPWriter(){
        delete writer;
        if(avoidDuplicates)
            delete history;
        close();
    };

    /**
      * open the pipe
      * blocks until an application opens the other end of the pipe
      * @return open success
      */
    bool open();
    /**
    * write the pcap file Header to the outgoing pipe
    * each reader shall call this method.
    * the method checks if the file header has already been written to the outgoing pipe in this case nothing happen
    *
    * the method uses mutex to avoid simultaneous writing
    * @param buffer  array to write to out pipe size <pcap.FileHeaderSize>
    */
    void writePCAPFileHeader(char *buff){
        mutex1.lock();
        if(!hasPCAPHeader){
            writer->write(buff,pcap::FileHeaderSize);
            hasPCAPHeader = true;
            Log::message(name,"write PCAP file header to outgoing pipe",2);
        }
        mutex1.unlock();

    }

    /**
     * if you have searched until a specif signature to find the PCAP file Header and those those byte are not anymore
     * in you buffer you can use this methode
     * this add(in case its not written jet)mutex protectet the file header to the OutPipe
     */
    void writePCAPFileHeader(char *buff1, int size1, char *buff2, int size2){
        mutex1.lock();
        if(!hasPCAPHeader){
            writer->write(buff1,size1);
            writer->write(buff2,size2);
            hasPCAPHeader = true;
            Log::message(name,"write PCAP file header to outgoing pipe",2);
        }
        mutex1.unlock();
    }

    /**
     * write an hole pcap to the out going pipe
     * to avoid simultaneously writes to the outgoing pipe an mutex locks until the hole pcap
     * is writen to the out pipe     *
     * @param pcap  struct witch containing  PCAP information
     */
    void write(pcap::PCAP * pcap);

    /**
     * close the pcap writer and every thing behind it
     */
    void close(){
        writer->close();
    }

};


#endif //PIPERELAYV2_PCAPWRITER_H
