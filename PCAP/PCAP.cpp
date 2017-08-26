//
// Created by basti on 3/9/17.
//

#include "PCAP.h"
#include "LinuxEncalsulation.h"
int pcap::append(char c) {
    if(isHeader){
        if(isTime){
            pcapStruct->time[position] = c;
            position++;
            if(position == pcapTimeSize){ // end of Time
                //cout <<" finish TIme"<<endl;
                position = 0;
                isTime = false;
                isSizePackage = true;
            }
        }
        else if(isSizePackage){
            pcapStruct->sizeOfPacket[position] = c;
            position ++;
            if(position == pcapSizeSize){// end of package
                if(LinuxEncalsulation::noEncapsulation()) {

                    pcapStruct->sizeOfPacket[0] = pcapStruct->sizeOfPacket[0] - (char) 16;
                  //  cout << "snipp size"<<endl;
                }
                //cout <<" finish Size"<<endl;
                isSizePackage = false;
                issizePayload = true;
                position = 0;
            }
        }
        else if(issizePayload) {
                pcapStruct->sizeOfPayload[position] = c;
                position++;
                if (position == pcapSizeSize) { // end of payload
                    if(LinuxEncalsulation::noEncapsulation())
                        pcapStruct->sizeOfPayload[0] = pcapStruct->sizeOfPayload[0] -(char)16;
                    //cout <<" finish Header"<<endl;
                    isHeader = false;
                    position = 0;
                    pcapStruct->intSize = convertSizeToInt(pcapStruct->sizeOfPacket);
                    if (pcapStruct->intSize > pcap::payloadSize)
                        return -2; // to big package
                }

        }
    }else{
        if(position > pcapStruct->intSize) {
            //cout <<" Error Payload"<<endl;
            isComplead = false;
            return -1;
        }
        if(LinuxEncalsulation::noEncapsulation() && encapsulationCounter < LinuxEncalsulationSize) {
            encapsulationCounter++;
            //cout << "cutout"<<endl;
        }
        else {
            //cout << "no cutout"<<endl;
            pcapStruct->payload[position] = c;
            position++;
            if (position == pcapStruct->intSize) {
                //cout <<" finish Payload"<<endl;
                isComplead = true;
                return 1;
            }
        }
    }
    return 0;

}

int pcap::convertSizeToInt(char *size) {
    int out = (unsigned char)size[0]<<0 | (unsigned char)size[1]<<8 | (unsigned char)size[2]<<16 | (unsigned char)size[3]<<24;
    return out;
}
int pcap::append(char *c, int size) {
    int last = 0;
    for(int i = 0; i<size;i++ ){
        last = append(c[i]);
        if(last <0)
            return last;
    }
    return last;
}

bool pcap::validPCAP() {
        if(!firstPcapSend) {
            readerMutex.lock();
            firstPcapSend = true;
            strncpy(firstTimeStamp,pcapStruct->time,pcap::pcapTimeSize);
            readerMutex.unlock();

        }
        else if(pcapStruct->time[3] != firstTimeStamp[3]) {
            if (pcapStruct->time[3] + 1 != firstTimeStamp[3]){
                firstTimeStamp[3] = pcapStruct->time[3];
                cerr<<" timestamp has changed since the first captured pacp file   time stamp get adapted \t this shall not happen often"<<endl;
            }else
                return false;
        }
        return true;

}