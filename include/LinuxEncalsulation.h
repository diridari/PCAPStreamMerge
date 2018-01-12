//
// Created by basto on 5/7/17.
//

#ifndef PIPERELAYV2_LINUXENCALSULATION_H
#define PIPERELAYV2_LINUXENCALSULATION_H


/**
 * This Class has the goal to mange the unwraping of linux encalsulated Pcap packages
 */
class LinuxEncalsulation {

public:
/**
   * if the network Protocol is not knowen to Linux it use an encapsulation Protocal
      but this encapsulation prococol canot be decoded by foren6
   * @return shall der be no encapsulation
   */
    static bool getEncapsulation();

    /**
     * set the Flag if the Linux Coocked encalsulation shall be cutout
     *
     * @param set true = cut out
     *          false = let encap Header in the pcap
     */
    static void setNoEncapsulation(bool set = false) ;


};


#endif //PIPERELAYV2_LINUXENCALSULATION_H
