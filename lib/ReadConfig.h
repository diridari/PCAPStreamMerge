//
// Created by basti on 3/3/17.
//

#ifndef THREADPIPE_READCONFIG_H
#define THREADPIPE_READCONFIG_H

#include <string>
#include <iostream>
#include "Log.h"

using namespace std;


/**
 * this class interpret an config file with name and/or location is disposaled to the Constructor
 * the config file has this format:
 *
 *     <host/ip> <space> <username> <remote command>
 *     ...
 *     <EOF or empty line>
 *     e.g
 *     192.168.7.2 bbb sudo tcpdump port 80 -U -s0  -w-
 *
 *
 */
class ReadConfig {
public:
    struct entry{
        string client = ""; // remote host or ip address
        string user = "";   // user name
        string execute = "";// renote command
        string setUp = "";
        bool valid;         // is the enty valid that means that all string != ""

        /**
         * creat an new enrty
         * @param client  Remote Host or IP
         * @param user    username
         * @param execute command
         * @param valid   is vlaid
         */
        entry(string client, string user, string execute, bool valid, string setUp =""):client(client), execute(execute), user(user), valid(valid),setUp(setUp){};
    };
private:
    entry * next;           // next valid entry
    ifstream *reader;       //file PipeReader
    string *name;           //filename
    bool hasnext;   // is there an valid entry  first changed in open()
    bool hasNextTmp;        // tmp var
    int lineIndex;               // line index


    /**
     * read an line
     * @return char array max <CharPerLine> char's
     */
    string *readLine();

    /**
    * create an struct containing the host(ip oder dns-name) and the password
    * @param line from the config file <client> <space> <user> <pw> <space oder eol>
    * @param length  number of chars at <line>
    * @return struct containing client and pw
    */
    ReadConfig::entry * enceodeLine(string * line);

    /**
     * checks if an entry is valid and set that flag in the entry
     * @param that  entry struct
     * @return  is ore not
     */
    static bool isValid(entry *that);
public:

    /**
     * New Interpreter obj
     * name contains the file name and/or the location
     * @param name name and/or location
     */
    ReadConfig(string *name): name(name){
        lineIndex = 0;
        hasnext = false;
    }

    /**
     * open the file
     * checks if there is an valid entry and store that entry in next
     */
    void open();


    /**
     * search the next valid entry
     * it is not defined what happen if ther is no valid left,
     * so the user has to check via hasNext() if there is an entry left
     * @return  pointer to an struct containing the next valid entry
     */
    entry *getNextValid();



    /**
     * is there an new valid entry left
     * @return true/false
     */
    bool hasNext(){return hasnext;};





};


#endif //THREADPIPE_READCONFIG_H
