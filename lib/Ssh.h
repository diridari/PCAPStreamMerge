//
// Created by basti on 3/4/17.
//

#ifndef PIPERELAYV2_SSH_H
#define PIPERELAYV2_SSH_H


#include <iostream>
#include <stdio.h>
#include "ReadConfig.h"
#include "PipeWriter.h"
#include "BaseReader.h"

#include "Log.h"
using namespace std;

class entry;

class entry;

/**
 * this class has the goal to enable an remote connection.
 * this class support an embedded ssh connection
 *
 * it is necessary that the ssh connection does not need an password, because you should not store the password in unencrypted .conf file :)
 * So you need an key-file
 *
 * this class use the default ssh tool installed on you hast system, so you can check if the password less login is possible
 *
 * the remote command will be excecuted as root so it could be necessary to add the remote executed application to "visudo" file tp avoid an password request
 */
class ssh : public BaseReader{

    string remoteExecute = "";      // this command will be Executed at the remote host
    FILE *pipe;                     // pipe where the remote ssh date came in
    bool isOpen = false;


protected:
    string host = "";               // name or ip-address of the remote host
    string user = "";               // User name of the remote host
    string setUp = "";

public:
    /**
     * Constructor
     * @param host host ore ip address of the remote system
     * @param user user name at the remote system
     * @param remoteExecute
     * @param outPipe
     */
    ssh(string host, string user, string remoteExecute, string setUp = ""):host(host), user(user),remoteExecute(remoteExecute),setUp(setUp){ };
    ssh(ReadConfig::entry *inputEntry){
        user = inputEntry->user;
        host = inputEntry->client;
        remoteExecute = inputEntry->execute;
        remoteSetUp(inputEntry->setUp);
    }


    /**
     * create a ssh connection to the host System
     * @return success
     */
    bool open();

    /**
     * befor starting the remote command this command
     * gets executed to setup the device
     *
     */
    bool remoteSetUp(string command);
    /**
     * Loop.
     * runs until the remote host close the connection
     */
    void read(char * bufffer, int size);

    bool hasNext(){
       return isOpen; //TODO
    }

    void close(){

    }


};


#endif //PIPERELAYV2_SSH_H
