//
// Created by basto on 7/3/17.
//

#include "BasePCAPReader.h"
bool BasePCAPReader::nextPCAP(){
    if(!isOpen){
        cerr << name << "  is not open"<<endl;
        return false;
    }
    Log::message(name,"get new pcap",2);
    pcap *p = new pcap();
    char c;
    bool finsh = false;
    while(!finsh){
        reader->read(&c,1);
        int status = p->append(c);
        if(status <0)
            return false;
        else if(status == 1) {
            finsh = true;
            Log::message(name," send pcap do out",3);
            outWriter->write(p->pcapStruct);

        }
    }
    return true;
}


void BasePCAPReader::run() {
    Log::message(name,"thread is  in run",3);
    if(!isOpen){
        cerr << name << "  is not open"<<endl;
        return;
    }
    char buffer[pcap::FileHeaderSize];
    if(hasOverhead){
        Log::message(name,"has overhead --> cout it out",2);
        cutOverhead();
        char sig[pcap::FileHeaderSize] = {(char)0xD4,(char)0xC3,(char)0xB2,(char)0xA1};
        Log::message(name,"Read File Header",2);
        reader->read(buffer,(pcap::FileHeaderSize-4));
        outWriter->writePCAPFileHeader(sig,4,buffer,pcap::FileHeaderSize-4);
    }
    else{
        Log::message(name,"Read File Header",2);
        reader->read(buffer,pcap::FileHeaderSize);
        outWriter->writePCAPFileHeader(buffer);
    }


    Log::message(name,"Found File Header --> goto PCAP packages",2);

    while(reader->hasNext()){

        if(!nextPCAP()){
            Log::message(name,"PCAP maye be invalid \t \t     >_< ",1 );
        }
        else
            Log::message(name,"found valid PCAP package ",3);


    }

}
void BasePCAPReader::cutOverhead() {
    char sig[pcap::FileHeaderSize] = {(char)0xD4,(char)0xC3,(char)0xB2,(char)0xA1};
    reader->readUntilSignatur(sig,4);
    Log::message(name,"found searched signature",3);

}