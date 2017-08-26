//
// Created by basto on 5/7/17.
//

#include "LinuxEncalsulation.h"


static bool  noLinuxCookEncapsulation = true;   // if the network Protocol is not knowen to Linux it use an encapsulation Protocal

bool LinuxEncalsulation::noEncapsulation() { return noLinuxCookEncapsulation; }


void  LinuxEncalsulation::setNoEncapsulation() { noLinuxCookEncapsulation = false;}