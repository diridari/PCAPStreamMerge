//
// Created by basto on 7/3/17.
//

#ifndef PIPERELAYV2_PCAPREADER_H
#define PIPERELAYV2_PCAPREADER_H


#include "../../../../Pipe-Relay/PipeRelay/Reader/PipeReader.h"
#include "../../../PCAP.h"
#include "../../Writer/PCAPWriter.h"
#include "../BasePCAPReader.h"

class PCAPPipeReader : public BasePCAPReader {
public:
    PCAPPipeReader(string location, string name, PCAPWriter *outPipe);;

public:


};


#endif //PIPERELAYV2_PCAPREADER_H
