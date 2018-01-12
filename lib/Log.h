//
// Created by basti on 4/19/17.
//

#ifndef PIPERELAYV2_LOG_H
#define PIPERELAYV2_LOG_H

#include <iostream>
using namespace std;
static int pCLI = 0;
class Log {


public:
    static void message(string name, string message, int prio );

    static void setPrio(int prioCLI);

};


#endif //PIPERELAYV2_LOG_H
