//
// Created by basto on 7/3/17.
//

#include "IO/Reader/BasePCAPReader.h"
bool BasePCAPReader::nextPCAP(){
    if(!isOpen){
        cerr << name << "  is not open"<<endl;
        return false;
    }
    Log::log(name +"get new pcap",Info);
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
            Log::log(name +" send pcap do out",Message);
            outWriter->write(p->pcapStruct);

        }
    }
    return true;
}


void BasePCAPReader::run() {
    Log::log(name +"thread is  in run",Debug);
    if(!isOpen){
        cerr << name << "  is not open"<<endl;
        return;
    }
    char buffer[pcap::FileHeaderSize];
    if(hasOverhead){
        Log::log(name +"has overhead --> cout it out",Info);
        cutOverhead();
        char sig[pcap::FileHeaderSize] = {(char)0xD4,(char)0xC3,(char)0xB2,(char)0xA1};
        Log::log(name +"Read File Header",Info);
        reader->read(buffer,(pcap::FileHeaderSize-4));
        outWriter->writePCAPFileHeader(sig,4,buffer,pcap::FileHeaderSize-4);
    }
    else{
        Log::log(name +"Read File Header",Info);
        reader->read(buffer,pcap::FileHeaderSize);
        outWriter->writePCAPFileHeader(buffer);
    }


    Log::log(name +"Found File Header --> goto src packages",Info);

    while(reader->hasNext()){

        if(!nextPCAP()){
            Log::log(name +"src maye be invalid \t \t     >_< ",Message );
        }
        else
            Log::log(name +"found valid src package ",Debug);


    }

}
void BasePCAPReader::cutOverhead() {
    char sig[pcap::FileHeaderSize] = {(char)0xD4,(char)0xC3,(char)0xB2,(char)0xA1};
    reader->readUntilSignatur(sig,4);
    Log::log(name +"found searched signature",Debug);

}