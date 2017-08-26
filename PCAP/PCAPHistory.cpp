//
// Created by basti on 2/28/17.
//

#include "PCAPHistory.h"
using namespace chrono;


size_t PCAPHistory::makeHash(pcap::PCAP *pcap) {
  /*  string * s = new string(pcap->payload);
    size_t h = hash_fn(*s);
    delete s;
    return h;
   */

    size_t v = 0;
    for(int i = 0; i< pcap->intSize; i++){

            v+= (((int)pcap->payload[i]) + 7*(i+1)) *(i+1);
    }
   // cout << "generate hash : "<<v <<endl;

    return v;
}

bool PCAPHistory::addTOList(size_t hash) {
    milliseconds ms = duration_cast< milliseconds >(
            system_clock::now().time_since_epoch()
    );
    time_t time = ms.count();
    if(time == -1)
        return false;
    Log::message("PCAP History"," add package to history",3);
    vector->insert(vector->begin(), new myHash(hash,time));
    return true;
}

void PCAPHistory::removeFromList(size_t hash) {
    for(int i = 0; i< vector->size(); i++){
        if(vector->at(i)->value == hash) {
           removeFromList(i);
            break;
        }
    }

}
bool PCAPHistory::removeFromList(int index) {
    Log::message("PCAP History" ,"delete package to history",3);
   myHash *that = vector->at(index);
    vector->erase(vector->begin() + index);
    delete that;

}


bool PCAPHistory::IsInList(pcap::PCAP *pcap) {

   // printVector();
    size_t hash = makeHash(pcap);
    milliseconds ms = duration_cast< milliseconds >(
            system_clock::now().time_since_epoch()
    );
    size_t now = ms.count();

    if(now == -1) {
        cerr << "error with Timestamp" << endl;
        return false;
    }
    time_t tmp = now - saveTime; //  is the saved time smaller than tmp it is out of date
    if(vector->size() >0) {

        for (int i = 0; i < vector->size(); i++) {
            if (vector->at(i)->time+saveTime < now) {
                //cout << "out of date  time since start" << now - vector->at(i)->time << endl;
                removeFromList(i);

            } else if (vector->at(i)->value == hash)
                return true;
        }
    }
    addTOList(hash);
    return false;

}
