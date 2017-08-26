//
// Created by basti on 4/19/17.
//

#include "Log.h"

// \033[1;31mbold
// \033[0m
// lolo
 void Log::setPrio(int prioCLI){
    pCLI = prioCLI;
};

 void Log::message(string name, string message, int prio) {
    if(prio <= pCLI) {
        switch (prio){
            case 1: cout << "\033[1;31m";break;
            case 2: cout << "\033[1;32m";break;
            case 3: cout << "\033[0;37m";break;
            default: cout << "\033[0;37m";break;
            }
        cout << "Message from : " << name << "  : " << message << endl;
        cout << "\033[0;0m";
    }
}