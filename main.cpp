//
// Created by Sebastian Balz  on 1/13/17.
//




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

#include <unistd.h>
#include "main.h"

#include "IO/Reader/PCAPPipeReader.h"


int main(int argc, char *argv[]) {

    configuration(argc,argv); // read parameter

    // open Writer
    (new string(location + "Out"), new string("PipeWriter"),(historyTime != 0),historyTime);
    pw = new PipeWriter(&location,new string("PipeWriter"));
    wr = new PCAPWriter(pw,"PCAPWriter",(historyTime != 0),historyTime);
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
    for(uint i = 0; i<l->size(); i++){
        l->at(i)->join();
    }
    return 0;

}




void handleNamedPipe(int numberOfThreads){
    Log::message("main",("cread " + to_string(numberOfThreads )+" named pipes" ),1);

    threadStartPipe *w;

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
    //  PCAPPipeReader *r = new PipeReader(w->location,w->outWriter,w->name,w->message, w->log);
    //PipeReader *pr = new PipeReader(w->location, new string("pipe"+ *w->name),w->message,w->log);
    PCAPPipeReader *r = new PCAPPipeReader(*w->location,*w->name,wr);
    r->open();
    r->run();
    Log::message("main:pipe", "\t\t"+ *w->name+ "    : leaf",1);
    delete w;
    delete r;
    return;
}


void handleSSH(string s){
    Log::message("main","open ssh with config file: \t" +s,1);


    ReadConfig *r = new ReadConfig(&s);
    r->open();

    while(r->hasNext()){
        ReadConfig::entry *entry = r->getNextValid();

        Log::message("main","establish new ssh conection to : \033[1;34m"+entry->client+ "\033[0m   with the user : \033[1;34m"
                            +entry->user +"\033[0m   and run there : [ \033[1;34m" + entry->execute+"\033[0m ]",1);

        PCAPSSHReader *pcapssh = new PCAPSSHReader(wr, "ssh to "+entry->client,entry);
        l->push_back(new thread(startSSHReader, pcapssh));

    }



}
void startSSHReader(PCAPSSHReader *s){
    if (remoteSetupIWPAN) {
        if (!s->runConfigIWPAN(channel, "phy0", "wpan0")) {
            cerr << "error while setup" << endl;
        }
    }
    if(remoteSetup){
        Log::message("main","send ssh setupt" ,2);
        s->executeCommand(remoteSetupCommand);
    }
    {
        Log::message("main:ssh","startThread",2);
        s->open();
        s->run();
    }
    delete s;
}

void  printUsage(){
    cout<<"usage:\t\033[1;32m"
        <<"\n\t[-ssh <configfile> ] \t\tconnect to ssh clients with the givven config"
        <<"\n\t[-pipe <nuberOf>]    \t\topen <numberOf named pipes und store them at <location>"
        <<"\n\t[-l <location>]      \t\tchange the default location to <location> and apend In and Out<n> default is /tmp/pip /tmp/pipeRelay"
        <<"\n\t[-ch <channel>       \t\tset the Channel (default: 26) just used by -ssh"
        <<"\n\t[-log <loglevel>]    \t\tset loglevel default : 0"
        <<"\n\t[-history <time>]    \t\tenables the pcap history"
        <<"\n\t[-encap]             \t\tset to disable the removing of the Linux Cooked Encapsulation "
        <<"\n\t[-remoteSetup < \"command  list of Param\">] \t\texecuting a script a the remote device if you add a \" "
                << "infront of the command and at its end everything inside gets send"
        <<"\n\t[-remoteSetupIWPAN]   \t\tenables the setup of the ssh device with before executing the remote command"

        <<"\033[0m"<<endl;

    exit(-1);

}
void wrongsage(){
    cout<<"wrong usage!\t use : \033[1;31m"
        <<endl;
    printUsage();
    exit(-1);
}


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
        else if (*s == "-ch")
            channel = argv[i + 1];
        else if (*s == "-h")
            printUsage();
        else if (*s == "-log")
            Log::setPrio(atoi(argv[i + 1]));
        else if (*s == "-history")
            historyTime = atoi(argv[i + 1]);
        else if (*s == "-encap") {
            LinuxEncalsulation::setNoEncapsulation();
            i--;
        } else if (*s == "-remoteSetup") {

            remoteSetup = true;
            string buildCommand = "";
            if(i + 1 >= numberOfArg){
                cerr<< "got more arguments with ssh command that expected do you have forgotten the \""<<endl;
            }
            buildCommand = argv[i + 1];
            if(buildCommand.at(0) ==  '\"' && buildCommand.at(buildCommand.length()-1 ) != '\"' ){
                buildCommand.front() = ' '; // remote "

                bool foundEnd = false;
                int pos = 2;
               while(!foundEnd){
                    pos ++;
                   i++;
                   if(i + pos >= numberOfArg){
                       cerr<< "got more arguments with ssh command that expected do you have forgotten the \""<<endl;
                   }
                   string nxt = argv[i + pos];

                   if(nxt.at(nxt.length()-1) == '\"'){
                       nxt.back() = ' ';  // remote " at the end
                       foundEnd = true;
                   }
                   buildCommand += nxt;
                }
            }
            remoteSetupCommand = buildCommand;
            cout <<" command : "<< remoteSetupCommand <<endl;
        }else if (*s == "-remoteSetupIWPAN") {
            remoteSetupIWPAN = true;
            i--;
        }
        else
            wrongsage();
    }
    if (!(doPipe || doSSH)) {
        numberOfPipes = 2;
        Log::message("main", "there was no input configuration -> 2 pipes", 2);
    }
}