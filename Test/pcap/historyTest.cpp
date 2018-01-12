//
// Created by basto on 9/3/17.
//

#include <gtest/gtest.h>


#include "../../include/IO/Writer/PCAPWriter.h"
#include "../../include/PCAP.h"
#include "../../include/LinuxEncalsulation.h"


int8_t  pcapF2[] = {0x16 ,(char)0xEF ,(char)0xC4 ,0x4A  ,(char)0xB3 ,0x45 ,0x09 ,0x00 ,0x04 ,0,0,0,0x04,0,0,0 ,0x01 ,0x02 ,0x03 ,0x04}; // 4byte payload
int8_t  pcapF1[] = {0x16 ,(char)0xEF ,(char)0xC4 ,0x4A  ,(char)0xB3 ,0x45 ,0x09 ,0x00 ,0x04 ,0,0,0,0x04,0,0,0 ,0x0f ,0x02 ,0x03 ,0x04}; // 4byte payload

/*
 * Test for src History
 * basic test fot the pcap history function
 */





pcap::PCAP * createPCAP(bool that){
    pcap *p = new pcap();
    LinuxEncalsulation::setNoEncapsulation();
    int count = 1 ;
    int8_t * t;
    if(that)
        t = pcapF1;
    else t= pcapF2;
    while (p->append((char)t[count-1]) != 1){
        count ++;

    }
    return  p->pcapStruct;
}
TEST(history,savePcap){
    PCAPHistory *hs = new PCAPHistory(100);
    pcap::PCAP *pc = createPCAP(true);
    bool inL = hs->IsInList(pc);
    ASSERT_FALSE(inL);


}

TEST(history,saveDivPcap){
    PCAPHistory *hs = new PCAPHistory(100);
    pcap::PCAP *pc = createPCAP(true);
    bool inL = hs->IsInList(pc);
    ASSERT_FALSE(inL);
    pcap::PCAP *p2c = createPCAP(false);
    bool inL2 = hs->IsInList(p2c);
    ASSERT_FALSE(inL2);
}
TEST(history,saveSamePcap){
    PCAPHistory *hs = new PCAPHistory(100);
    pcap::PCAP *pc = createPCAP(true);
    bool inL = hs->IsInList(pc);
    ASSERT_FALSE(inL);
    pcap::PCAP *p2c = createPCAP(true);
    bool inL2 = hs->IsInList(p2c);
    ASSERT_FALSE(!inL2);
}


TEST(history,saveDivPcapTimeAfterTimeout){
    PCAPHistory *hs = new PCAPHistory(100);
    pcap::PCAP *pc = createPCAP(true);
    bool inL = hs->IsInList(pc);
    ASSERT_FALSE(inL);
    pcap::PCAP *p2c = createPCAP(false);
    bool inL2 = hs->IsInList(p2c);
    ASSERT_FALSE(inL2);
    usleep(200000);

     inL = hs->IsInList(pc);
    ASSERT_FALSE(inL);
     inL2 = hs->IsInList(p2c);
    ASSERT_FALSE(inL2);
}
TEST(history,saveSamePcapAfterTimeout){
    PCAPHistory *hs = new PCAPHistory(100);
    pcap::PCAP *pc = createPCAP(true);
    bool inL = hs->IsInList(pc);
    ASSERT_FALSE(inL);
    pcap::PCAP *p2c = createPCAP(true);
    bool inL2 = hs->IsInList(p2c);
    ASSERT_FALSE(!inL2);


    usleep(200000);
     inL = hs->IsInList(pc);
    ASSERT_FALSE(inL);
    inL2 = hs->IsInList(p2c);
    ASSERT_FALSE(!inL2);
}
