#ifndef THREADPIPE_PCAP_H
#define THREADPIPE_PCAP_H

#include <string.h>
#include <iostream>
#include <mutex>

//
// Created by Sebastian Balz  on 1/13/17.
//
/**
 * Important:
 * there is an difference between pcap-Header and pcap-file-Header
 * the file-header is 24byte big and contains information about the file
 * each inputstream has his own file header but the outgoing pipe shall have just one file header so each instram has to decide
 * if it shall cut this header out or send it to the writer
 *
 * the pcap header is an header for the payload
 * it contains information about the captured time size of the payload stored in the file and size of the real payload send on the wire
 * the pcap header is 16 byte big
 *  PCAP Header
 *  -   Time    (8byte)
 *  -   size    (4byte)
 *  -   size    (4byte)
 *  -   payload (size)
 */


using namespace std;

static bool FIRSTPIPEISOPEN = false;
static std::mutex readerMutex;                  // mutex : wird der Reader gerade verwendent
static bool firstPcapSend = false;              // wurde bereits ein src paket gesendet  wird genötigt um den Timstamp zu verifizieren
static char firstTimeStamp[8] = {};             // timestamp des ersten src paketes

                                                    // but this encapsulation prococol canot be decoded by foren6
static int LinuxEncalsulationSize = 16;




class pcap{
public:
    static const int FileHeaderSize = 24;    // size  of File Header
    static const int pcapTimeSize = 8;       // size in byte  of timestamp
    static const int pcapSizeSize = 4;       // size in bytes of the size(both real and stored)
    static const int payloadSize =  5000;    // max size of the payload(IEEE 802.15.4 MTU size is just 1280)



    int encapsulationCounter = 0;
    bool isComplead= false;
    /**
     * this struct describe an single pcap
     *
     */
    struct PCAP{
        char time [pcapTimeSize] = {};        // timestamp
        char sizeOfPacket[pcapSizeSize]= {} ; // size of stored Size
        char sizeOfPayload[pcapSizeSize]= {}; // size of readl Size
        bool isValid = false;                 // just a sign if the pcap is valid
        int intSize;                          // size as int
        char payload [payloadSize]= {};       // payload stored in char array
    };
private:
    bool isHeader = true;
    bool isTime = true;
    bool isSizePackage = false;
    bool issizePayload = false;
    int position = 0;


    /**
     * convert an Byte Array with the size 4 to an int value
     * atoi has problem with null elements
     * @param size
     * @return
     */
    int convertSizeToInt(char size[4]);

    bool validPCAP();
public:
    /**
     * stuct witch describes the size of each element
     */
    PCAP* pcapStruct = new PCAP();

    /**
     * append a singe char the this pcap element
     * @param c char
     * @return  < 0there are more chars than the cap as elements
     *          0 all ok append more chars
     *          1 pcap is complete
     */
    int append(char c);
    /**
 * append several chars the this pcap element
 * @param c char
 * @param size how many elements to add
 * @return  < 0there are more chars than the cap as elements
 *          0 all ok append more chars
 *          1 pcap is complete
 */
    int append(char *c, int size);



};

#endif //THREADPIPE_PCAP_H
