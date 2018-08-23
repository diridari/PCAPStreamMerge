//
// Created by basto on 7/3/17.
//

#include <PipeReader.h>
#include "IO/Writer/PCAPWriter.h"

#include "IO/Reader/PCAPPipeReader.h"
#include <Ssh.h>
#include "IO/Reader/PCAPSSHReader.h"

bool PCAPSSHReader::runConfigIWPAN(string channel, string phy, string wpan) {

    string t = "ssh -q " + config->user +"@"+config->client + " 'sudo /etc/IEEE-Sniffer/remoteSetup/setup.sh "
               + phy + " " + " " + wpan + " " + channel + " '";
    Log::log(("ssh config " + config->client)+(" : " +t) ,Info);
    FILE *setUp;
    char shallBe[]  = "end";
    char is[3];
    if(( setUp = popen((t.c_str()), "r"))){
        fread(is,3,1,setUp);
    }
    pclose(setUp);
    bool isSucess =  is[0] == shallBe[0] && is[1] == shallBe[1] &&is[2] == shallBe[2];
    if(!isSucess){
        Log::log(("ssh config " + config->client) + (" :  failt so setup client-config") ,Info);
    }
    else{
        Log::log(("ssh config " + config->client)+ (" : set up client-config to channel " +channel  +" succeed ") ,Info);
    }
    return isSucess;

}

void  PCAPSSHReader::executeCommand(string command) {
    FILE *setUp;
    string t = "ssh -q " + config->user +"@"+config->client + " 'sudo " + command + "'";
    Log::log(("ssh config " + config->client)+(" : " +t) ,Info);
    setUp = popen((t.c_str()), "r");
    pclose(setUp);
}

