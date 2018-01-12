//
// Created by basto on 7/3/17.
//

#ifndef PIPERELAYV2_PCAPSSHREADER_H
#define PIPERELAYV2_PCAPSSHREADER_H

#include "BasePCAPReader.h"
#include <Ssh.h>
#include <ReadConfig.h>


class PCAPSSHReader : public BasePCAPReader {
    ReadConfig::entry *config;
public:


    PCAPSSHReader( PCAPWriter *out, string name1, ReadConfig::entry *config) :
            BasePCAPReader(new ssh(config),out,name1), config(config) {


         };

        /**
        * this starts an script at the remote host witch sets up the NL802154 Sniffer
        * @param channel witch the sniffer is configured
        * @param phy default is phy0 but if there are several phy devices
        * @param wpan name of the new interface
        * @return was the setup successfull
        */
    bool runConfig(string channel,string phy = "phy0", string wpan = "wpan0");


};
#endif //PIPERELAYV2_PCAPSSHREADER_H
