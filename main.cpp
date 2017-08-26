
//
// Created by Sebastian Balz  on 1/13/17.
//


#include <search.h>
#include "main.h"
#include "PipeRelay/Reader/PipeReader.h"


using namespace std;

/**
 * PipeRelay.v.2
 * Version 2
 * support multithreading and pcap detection
 *
 * this application has the goal to connect several ingoing pipes with one outgoing pipe
 * The ingoing pipes are filled with remote captured from several probes. Each probes data is stored in one singe named pipe.
 * The data of each probe can be grabbed by an ssh connection witch is piping the tcpdump-data the named pipe

 * Wireshark e.g is able the read data from a singe named pipe,
 * but if there is more than on probe, the data of each probe has to be bundled to a singe outgoing pipe witch can be captured
 * by Wireshark or by an other application
 *
 *
 * This program is the binding link of ingoing pipes and the outpoing pipe.
 * Each ingoing pipe has an Thread witch is reader pcap package. If the pcap is completely read, the thread sends the pcap to the outgoing pipe
 * to avoid simultaneous writes to the outgoing pipe each write is locked with an mutex
 */
/*
 * - ssh configFile
 * - pipe number
 * - ch channel
 * - location
 * - log loglevel
 * - encap
 * - remoteSetup
 */
#include <unistd.h>


int main(int argc, char *argv[]) {


    configuration(argc,argv); // read parameter

    // open Writer

    wr = new PipeWriter(new string(location + "Out"), new string("PipeWriter"));

    wr->open();

    // open ssh
    if(doSSH){
        handleSSH(configFile);
    }
    // open named pipes
    if(numberOfPipes > 0)
        handleNamedPipe(numberOfPipes);

    sleep(1);

    // wait for finish
    for(int i = 0; i<l->size(); i++){
        l->at(i)->join();
    }
    return 0;

}



void handleSSH(string s){
    Log::message("main","open ssh with config file: \t" +s,1);


    ReadConfig *r = new ReadConfig(&s);
    r->open();

    while(r->hasNext()){
        ReadConfig::entry *entry = r->getNextValid();

        Log::message("main","establish new ssh conection to : \033[1;34m"+entry->client+ "\033[0m   with the user : \033[1;34m"
                            +entry->user +"\033[0m   and run there : [ \033[1;34m" + entry->execute+"\033[0m ]",1);
        ssh *s = new ssh(entry,wr);
        delete entry;
        l->push_back(new thread(startSSHReader, s));
    }



}
void handleNamedPipe(int numberOfThreads){
    Log::message("main",("cread " + to_string(numberOfThreads )+" named pipes" ),1);

    threadStartPipe *w;
    thread *list[numberOfThreads];

    // creat all threads
    for(int i = 0; i <numberOfThreads; i++) {
        string *loc = new string(location + "In" + to_string(i));
        string *name = new string("Reader: " + to_string(i));
        w = new threadStartPipe(loc, wr, name);

        l->push_back(new thread(startPipeReader, w));
    }




}


void startPipeReader(threadStartPipe *w){
    Log::message("main:pipe","startThread",2);
    PipeReader *r = new PipeReader(w->location,w->outWriter,w->name,w->message, w->log);
    r->open();
    while(true){
        char c;
        r->read(&c,1);
        wr->write(&c,1);
    }
    Log::message("main:pipe", "\t\t"+ *w->name+ "    : leaf",1);
    delete w;
    delete r;
    return;
}

void startSSHReader(ssh *s){

        Log::message("main:ssh","startThread",2);
        s->open();
    while(true){
        char c;
        s->read(&c,1);
       wr->write(&c,1);
    }



    delete s;
}


void  printUsage(){
    cout<<"usage:\t\033[1;32m"
        <<"\n\t[-ssh <configfile> ] \t\tread config file  "
        <<"\n\t[-pipe <nuberOf>]    \t\topen <numberOf named pipes und store them at <location>"
        <<"\n\t[-l <location>]      \t\tchange the default location to <location> and apend In and Out<n>"
        <<"\n\t[-log <loglevel>]    \t\tset loglevel default : 0"
        <<"\033[0m"<<endl;

    exit(-1);

}
void wrongsage(){
    cout<<"wrong usage!\t use : \033[1;31m"
        <<endl;
    printUsage();
    exit(-1);
}
/*
 * -ssh configFile
 * -pipe number
 * -ch channel
 * -l location
 */
void configuration(int numberOfArg, char *argv[]) {

    Log::message("main", "read configuration", 3);
    for (int i = 1; i < numberOfArg; i += 2) {
        bool hasNext = i + 1 < numberOfArg;
        string *s = new string(argv[i]);
        if (*s == "-ssh" & hasNext) {
            doSSH = true;
            configFile = string(argv[i + 1]);
        } else if (*s == "-pipe" & hasNext) {
            numberOfPipes = atoi(argv[i + 1]);
            doPipe = true;
        } else if (*s == "-l" & hasNext)
            location = string(argv[i + 1]);
        else if (*s == "-h")
            printUsage();
        else if (*s == "-log")
            Log::setPrio(atoi(argv[i + 1]));
        else if (*s == "-history")
            Log::setPrio(atoi(argv[i + 1]));
       else
            wrongsage();
    }
    if (!(doPipe || doSSH)) {
        numberOfPipes = 2;
        Log::message("main", "there was no input configuration -> 2 pipes", 2);
    }
}

