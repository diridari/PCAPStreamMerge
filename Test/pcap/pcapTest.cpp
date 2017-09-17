//
// Created by basto on 9/3/17.
//



#include <gtest/gtest.h>

#include "../../PCAP/IO/Writer/PCAPWriter.h"
#include "../../PCAP/PCAP.h"
#include "../../PCAP/LinuxEncalsulation.h"


int8_t pcapF3[] = {0x16, (char) 0xEF, (char) 0xC4, 0x4A, (char) 0xB3, 0x45, 0x09, 0x00, 0x04, 0, 0, 0, 0x04, 0, 0, 0,
                  0x01, 0x02, 0x03, 0x04}; // 4byte payload
int8_t encap[] = {0x16, (char) 0xEF, (char) 0xC4, 0x4A, (char) 0xB3, 0x45, 0x09, 0x00, 0x14, 0, 0, 0, 0x14, 0, 0, 0,
                   0x01, 0x02, 0x03, 0x04}; // 4byte payload

/*
 * Check that the building of a pcap struct works correct
 */
TEST(pcap, pcapBuild) {
    LinuxEncalsulation::setNoEncapsulation();
    pcap *p = new pcap();
    LinuxEncalsulation::setNoEncapsulation();
    int count = 1;
    while (p->append((char) pcapF3[count - 1]) != 1) {
        count++;

    }
    ASSERT_EQ(count, sizeof(pcapF3));


}

TEST(pcap, pcapPayload) {
    LinuxEncalsulation::setNoEncapsulation();
    pcap *p = new pcap();
    LinuxEncalsulation::setNoEncapsulation();
    int count = 1;
    while (p->append((char) pcapF3[count - 1]) != 1) {
        count++;

    }
    int8_t  payload[] = {0x01,0x02,0x03,0x04};
    ASSERT_EQ(p->pcapStruct->intSize,4);

    for (int i = 0; i <4 ; ++i) {
        ASSERT_EQ(payload[i],p->pcapStruct->payload[i]);
    }

}

TEST(pcap, AppendTooMuch) {

    pcap *p = new pcap();
    LinuxEncalsulation::setNoEncapsulation();
    for(int i = 0; i<sizeof(pcapF3)-1;i++)
        ASSERT_EQ(p->append(pcapF3[i]),0);
    ASSERT_EQ(p->append(0xFF),1);  // last payload
    ASSERT_EQ(p->append(0xAA),-1); // to much
    ASSERT_EQ(p->append(0xA),-1); // to much
    ASSERT_EQ(p->append(0x9A),-1); // to much


}

TEST(pcap,encapsulation){
    LinuxEncalsulation::setNoEncapsulation(true);
    uint8_t pcapEnd[] = {0x0,0x1,0x2,0x3,4,5,6,7,8,9,10,11,12,13,14,15}; // random struff
    pcap *p = new pcap();
    
    for(int i = 0; i< sizeof(encap);i++){
        ASSERT_EQ(p->append(encap[i]),0);
    }
    int i =0;
    while(p->append(pcapEnd[i])== 0){
        i++;
    }
    int8_t  payload[] = {12,13,14,15};
    ASSERT_EQ(p->pcapStruct->intSize,4);
    for (int i = 0; i <4 ; ++i) {
        ASSERT_EQ(payload[i],p->pcapStruct->payload[i]);
    }


}

