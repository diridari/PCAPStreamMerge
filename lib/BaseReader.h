//
// Created by basto on 6/26/17.
//

#ifndef PIPERELAYV2_BASEREADER_H
#define PIPERELAYV2_BASEREADER_H


class BaseReader {
public :

    /**
     * reads some Bytes frome the ingoing source and stores dem in the provided buffer
     * @param buffer pointer to start of the buffer   ensure that the buffer is big enough
     * @param size number of bytes to read
     */
    virtual void read(char * buffer, int size) = 0;

    /**
     * if the pipe has overhead with shall be ignored use this method to cut out everything until an given signature
     * this method does not check cycles in the signature(its reads until the next byte does not mach an restart at the
     * beginning of given signature) , so the user has to ensure that in the given signature are no cycles
     * WARNING:
     *  this also cut out the first <size> Byts of the content
     * @param buffer pointer to the given signature
     * @param size size of the signature
     */
    void readUntilSignatur(char * buffer, int size);

    /**
     * returns if ther migth be a new char in the source
     * in case of the a pipe this just return if the pipe is not closed
     */
    virtual bool hasNext() = 0;

    /**
   * open thePipe
   * blocks until the pipe is opend
   * @return pipe is opened
   */
    virtual bool open() = 0;

    /**
    * close the named pipe and delete the file
    */
    virtual void close() = 0;

};


#endif //PIPERELAYV2_BASEREADER_H
