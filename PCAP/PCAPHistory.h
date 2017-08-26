//
// Created by basti on 2/28/17.
//

#ifndef THREADPIPE_PCAPHISTORY_H
#define THREADPIPE_PCAPHISTORY_H

#include <iostream>
#include "PCAP.h"
#include <vector>
#include <ctime>
#include <chrono>
#include "../Loging/Log.h"
/**
 *  the goal of this class is to avoid duplicate pcap's send the the destianation pipe.
 * there is following  constellation:
 *  there are two sniffers and 3 sender
 *  Sender 1 has a connection to sender 2, and sender 2 has a connection to sender 3
 *  Sniffer 1 can only see the traffic created by sender 1 and sender 2 but not the data from sender 3
 *  Same here with sniffer 2 he can only see the data created by sender 2 and sender 3.
 *
 *  imagine  an package is send from 1 to 2 to 3:
 *  1 to 2 : sniffer 1 reads the data and sent it to the host
 *  2 to 3 : both sniffer 1 and sniffer 2 reads the data and send it to the host
 *
 *            the host gets one duplicate package
 *
 *  the goal of this class is to avoid duplicate pcap's send the the destianation pipe
 *
 */
 using namespace std;

class PCAPHistory {

    /**
     * struct to describe the hash value and his timestamp
     * to store it in an vector
     */
    struct myHash{
        size_t value;
        time_t time;
        myHash(size_t value, time_t time): time(time), value(value){};
    };


    /**
     * time how long an hash value gets stored
     */
    int saveTime;

    /**
     * Storage for the hash and timestamp
     */
    std::vector<myHash*> *vector;

    /**
     * delete the struct with the same hash value from the vector
     * @param hash  hash to del
     * @return  success
     */
    void removeFromList(size_t hash);

    /**
    * delete the struct with this index from the list
    * @param hash  hash to del
    * @return  success
    */
    bool removeFromList(int index);

    /**
     * add the hash and the timestamp to the vector
     *
     * @param hash
     * @return
     */
    bool addTOList(size_t hash);


    /**
     * create an pseudo hash value over the payload
     * the normal shark can not be used because it has trubble with "0" values
     * in the char array
     * each char gets multiplied with its index
     * @param pcap pcap the hash
     * @return pseudo hash
     */
    size_t makeHash(pcap::PCAP *pcap);


    /**
     * just for debugging
     * prints the vector on the CLI
     */
    void printVector(){
        for(int i = 0; i< vector->size();i++)
            cout <<"[ "<< vector->at(i)->value << " " <<vector->at(i)->time << " ]  ";
        cout <<endl;
    }
public:

    /**
     * Constructor
     * @param saveTime time how long an data is stored  in milliseconds  default 2000ms
     */
    PCAPHistory(unsigned int  saveTime = 2000):saveTime(saveTime) {
       vector = new std::vector<myHash*>();

    };

    /**
     * checks if there was an identical pcap in the last <saveTime> seconds
     * @param pcap  pcap to check
     * @return  true if there was an identical pcap
     */
    bool IsInList(pcap::PCAP *pcap);
};


#endif //THREADPIPE_PCAPHISTORY_H
