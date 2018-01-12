//
// Created by basto on 7/3/17.
//

#include "../../../include/IO/Writer/PCAPWriter.h"


bool PCAPWriter::open(){

    isOpen =  writer->open();
    return  isOpen;
}

/**
 * function witch write an pcap to an certain ofstream
 * @param pcap  pcap struct
 * @param out   ofstream
 */
void writerToOfstream(pcap::PCAP * pcap, PipeWriter *out){

    out->write(pcap->time, pcap::pcapTimeSize);
    out->write(pcap->sizeOfPacket, pcap::pcapSizeSize);
    out->write(pcap->sizeOfPayload, pcap::pcapSizeSize);

    out->write(pcap->payload,pcap->intSize);
   
}

void PCAPWriter::write(pcap::PCAP * pcap) {
    if(!isOpen){
        cerr << name << " write() is not open"<<endl;
        return;
    }
    Log::message(name,"lock mutex",3);
    mutex1.lock();  // set mutex
    Log::message(name,"write src",3);

    if(!(avoidDuplicates && history->IsInList(pcap))) {  // avoid duplicate pcap's
        writerToOfstream(pcap, writer);


    }
    else {
        Log::message(name," detect duplicate pcap --> cut it out",2);
    }
    Log::message(name,"free mutex",3);
    mutex1.unlock(); // free mutex
}