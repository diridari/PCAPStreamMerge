//
// Created by basto on 7/3/17.
//

#include "PCAPPipeReader.h"

PCAPPipeReader::PCAPPipeReader(string location, string name, PCAPWriter *outPipe) :
        BasePCAPReader(new PipeReader(&location,&name),outPipe, name){


}
