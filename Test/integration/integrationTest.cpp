//
// Created by basto on 9/3/17.
//

#include "../../PipeRelay/PipeRelay/Writer/PipeWriter.h"
#include "../../PCAP/IO/Writer/PCAPWriter.h"
#include "../../PCAP/IO/Reader/Pipe/PCAPPipeReader.h"
#include "../../PCAP/LinuxEncalsulation.h"
#include <unistd.h>
#include <gtest/gtest.h>

int8_t pcapFileHEader[] = {(char) 0xD4, (char) 0xC3, (char) 0xB2, (char) 0xA1, 0x02, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00,
                           0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0, 1, 0, 0, 0};
int8_t pcapF[] = {0x16, (char) 0xEF, (char) 0xC4, 0x4A, (char) 0xB3, 0x45, 0x09, 0x00, 0x04, 0, 0, 0, 0x04, 0, 0, 0,
                  0x01, 0x02, 0x03, 0x04}; // 4byte payload



/*
 * Basic piep Integration Test
 * the idea is to run a the application and open the in an output -pipes by File Reader to fill them
 * this has can only be done with threads because the reader/writer blocks while opening
 *
 */

void reader(string loc, PCAPWriter *pw) {
    PCAPPipeReader *r = new PCAPPipeReader(loc, "reader", pw);
    r->open();
    r->run();

}

void writer(PCAPWriter *pcw) {
    pcw->open();


}


void PipeFiller(string loc, int numberOfPCAPs) {
    ofstream *f = new ofstream();
    f->open(loc);

    f->write((char *) pcapFileHEader, sizeof(pcapFileHEader));
    for (int i = 0; i < numberOfPCAPs; i++) {
        int del = random()&100;
        usleep(del);
        for (int x = 0; x < sizeof(pcapF); x++) {
            f->put(pcapF[x]);
            f->flush();
            usleep(1);

        }
    }
}

TEST(shall, ok) {
    ASSERT_EQ(1, 1);
}

TEST(integrationTEst, oneReader) {
    Log::setPrio(-1);
    LinuxEncalsulation::setNoEncapsulation();
    PipeWriter *pw = new PipeWriter(new string("/tmp/testWr"));
    PCAPWriter *pcw = new PCAPWriter(pw, "testWriter");
    thread *t = new thread(writer, pcw);
    usleep(100000);
    ifstream *in = new ifstream();
    in->open("/tmp/testWr");
    usleep(100000);
    thread *r1 = new thread(reader, "/tmp/testReader0", pcw);
    usleep(100000);
    thread filler(PipeFiller, "/tmp/testReader0", 1);
    filler.join();

    for (int i = 0; i < sizeof(pcapFileHEader); i++) {
        char c = in->get();
        ASSERT_EQ((uint8_t) c, (uint8_t) pcapFileHEader[i]);
    }
    for (int i = 0; i < sizeof(pcapF); i++) {
        char c = in->get();
        ASSERT_EQ((uint8_t) c, (uint8_t) pcapF[i]);
    }

}

TEST(integrationTEst, twoReader) {
    Log::setPrio(-1);
    LinuxEncalsulation::setNoEncapsulation();
    PipeWriter *pw = new PipeWriter(new string("/tmp/testWr"));
    PCAPWriter *pcw = new PCAPWriter(pw, "testWriter");
    thread *t = new thread(writer, pcw);
    usleep(100000);
    ifstream *in = new ifstream();
    in->open("/tmp/testWr");
    usleep(100000);
    thread *r1 = new thread(reader, "/tmp/testReader0", pcw);
    thread *r2 = new thread(reader, "/tmp/testReader1", pcw);
    usleep(100000);

    thread filler(PipeFiller, "/tmp/testReader0", 1);
    thread filler2(PipeFiller, "/tmp/testReader1", 1);


    filler.join();
    filler2.join();

    for (int i = 0; i < sizeof(pcapFileHEader); i++) {
        char c = in->get();
        ASSERT_EQ((uint8_t) c, (uint8_t) pcapFileHEader[i]);
    }
    for (int x = 0; x < 2; x++) {
        for (int i = 0; i < sizeof(pcapF); i++) {
            char c = in->get();
            ASSERT_EQ((uint8_t) c, (uint8_t) pcapF[i]);
        }
    }

}


TEST(integrationTEst, twoReaderSeveralPcaps) {
    Log::setPrio(-1);
    LinuxEncalsulation::setNoEncapsulation();
    PipeWriter *pw = new PipeWriter(new string("/tmp/testWr"));
    PCAPWriter *pcw = new PCAPWriter(pw, "testWriter");
    thread *t = new thread(writer, pcw);
    usleep(10000);
    ifstream *in = new ifstream();
    in->open("/tmp/testWr");
    usleep(10000);
    thread *r1 = new thread(reader, "/tmp/testReader0", pcw);
    thread *r2 = new thread(reader, "/tmp/testReader1", pcw);
    thread *r3 = new thread(reader, "/tmp/testReader2", pcw);
    thread *r6 = new thread(reader, "/tmp/testReader3", pcw);
    thread *r4 = new thread(reader, "/tmp/testReader4", pcw);
    thread *r5 = new thread(reader, "/tmp/testReader5", pcw);
    usleep(10000);

    thread filler(PipeFiller, "/tmp/testReader0", 100);
    thread filler2(PipeFiller, "/tmp/testReader1", 100);
    thread filler3(PipeFiller, "/tmp/testReader2", 100);
    thread filler1(PipeFiller, "/tmp/testReader3", 100);
    thread filler4(PipeFiller, "/tmp/testReader4", 100);
    thread filler5(PipeFiller, "/tmp/testReader5", 100);
    thread filler6(PipeFiller, "/tmp/testReader6", 100);

    filler.join();
    filler1.join();
    filler2.join();
    filler3.join();
    filler4.join();
    filler5.join();
    filler6.join();

    for (int i = 0; i < sizeof(pcapFileHEader); i++) {
        char c = in->get();
        ASSERT_EQ((uint8_t) c, (uint8_t) pcapFileHEader[i]);
    }
    for (int x = 0; x < 600; x++) {
        for (int i = 0; i < sizeof(pcapF); i++) {
            char c = in->get() & 0xff;
            ASSERT_EQ((uint8_t) c, (uint8_t) pcapF[i]);

        }
    }

}


