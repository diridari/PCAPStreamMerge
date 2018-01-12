//
// Created by basto on 7/3/17.
//

#include <PipeReader.h>
#include "../../../../include/IO/Writer/PCAPWriter.h"

#include "../../../../include/IO/Reader/PCAPPipeReader.h"
#include <Ssh.h>
#include "../../../../include/IO/Reader/PCAPSSHReader.h"

bool PCAPSSHReader::runConfigIWPAN(string channel, string phy, string wpan) {

    string t = "ssh -q " + config->user +"@"+config->client + " 'sudo /etc/IEEE-Sniffer/remoteSetup/setup.sh "
               + phy + " " + " " + wpan + " " + channel + " '";
    Log::message(("ssh config " + config->client),(" : " +t) ,2);
    FILE *setUp;
    char shallBe[]  = "end";
    char is[3];
    if(( setUp = popen((t.c_str()), "r"))){
        fread(is,3,1,setUp);
    }
    pclose(setUp);
    bool isSucess =  is[0] == shallBe[0] && is[1] == shallBe[1] &&is[2] == shallBe[2];
    if(!isSucess)
        Log::message(("ssh config " + config->client),(" failt so setup client-config") ,1);
    else
        Log::message(("ssh config " + config->client),("set up client-config to channel " +channel  +" succeed ") ,2);
    return isSucess;

}

void  PCAPSSHReader::executeCommand(string command) {
    FILE *setUp;
    string t = "ssh -q " + config->user +"@"+config->client + " 'sudo " + command + "'";
    Log::message(("ssh config " + config->client),(" : " +t) ,2);
    setUp = popen((t.c_str()), "r");
    pclose(setUp);
}

