//
// Created by basto on 7/3/17.
//

#include "IO/Writer/PCAPWriter.h"


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
    Log::log(name +"lock mutex",Debug);
    mutex1.lock();  // set mutex
    Log::log(name +"write src",Debug);

    if(!(avoidDuplicates && history->IsInList(pcap))) {  // avoid duplicate pcap's
        writerToOfstream(pcap, writer);


    }
    else {
        Log::log(name +" detect duplicate pcap --> cut it out",Info);
    }
    Log::log(name +"free mutex",Debug);
    mutex1.unlock(); // free mutex
}