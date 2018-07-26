# PCAP Stream Merge 
## Goal   
This application has the goal to combine the traffic of several Network devices to one outgoing date stream. 
This allows you to simplify the analytics and the bug finding of mesh-networks.

You may use this application if you develop IEE 802.15.4 (IoT) devices or other kinds of Mesh networks
 where it is not possible to capture the whole network by one single sniffing-device.  

The Host requests the tcpdump (or another pcap source) from the Clients, reads and combines them.
There are some basic filters that allow suppressing double received packages, e.g. if one datapackage has been captured by two 
sniffers.

This application is based on Pipe-Relay, which was written to combine several ingoing pipes to one outgoing pipe.

![Alt text](pic/img.PNG?raw=true "structure")

***
## Usage
start this programm from the cli. 
you can use these parameter:
            
                    -ssh <configfileName/location>      connect to ssh clients with the given config
                    -pipe <nuberOf>                     open <numberOf> named pipes und store them at <location>
                    -l <location>                       change the default location to <location> and append In and Out<n> default is /tmp/pip /tmp/pipeRelay
                    -ch <channel>                       set the Channel (default: 26) used by -ssh
                    -log <loglevel>                     set loglevel default : 0     the higher the more info you will get 
                    -history <time>                     enables the pcap history     time in ms 
                    -encap                              set to disable the removing of the Linux Cooked Encapsulation 
                    -noRemoteSetup                      disable the setup of the ssh device before executing the remote command


            
   < configFile > is the name and/or the location of the file witch describe the ssh connection (see: "how it work's::ssh").
   
   < number of > is the number of named pipes, which the programm will open.   
          The named pipes are stored at "/tmp/pipeRelay< number >" or if given at the location provided by -l   
          If there is no parameter the application creates 3 named-pipes by default
          
   "encap"sulation: linux encapsulation network packages from an unknown source. Foren6 is not able to handle such packages
   the solution in this case is to remove this encapsulation header and change the pcap-linkylaer to a 6lowpan one.
   By default this feature is enabled.
   
   If you want to connect to a remote device via SSH it is recommended to enable a connection without password using 
   public/private key as authentication. 

***
## How it works
The application opens one outgoing pipe which gets packed with the combined ingoing traffic (the program blocks until 
the pipe has read "end").    
Then for each target device (named pipe at the local machine or remote with SSH) the program opens one tunnel.   
* if the device is a named pipe: the content is pcap
* if the device is a SSH connection: the application connects to the devices defined in the config file.
### Config-File
If the application runs in ssh mode, the user has to define a config file. Using this file the app can read which clients to 
connect to and which applications are supposed to run at the remote client.   
The config file has following definitions:   
- each entry stands in a separate line
- a space (' ') separates the entry-parts
- an empty line or the EOF descriptor define the end of the config file 
- there are max 200 chars allowed per line   

#### Syntax:   
* < hostname or ip> < space> < username> < space> < remote command>    <line end>

* < hostname or ip> < space> < username> < space> < remote command>   <line end>

* ...   
   
e.g   

          127.0.0.1 linuxUser sudo tcpdump -i eth0
          myServer  sudo tcpdump -i eth0  // invalid          
          8.8.8.8 google please hack that for me :)   
          
* the first line says that the host ist "127.0.0.1" the user is "LinuxUser" and the command is "sudo tcpdump -i eth0"   
* the second line says that the host ist "myServer" the user is empty(2 spaces)   and the command is "tcpdump -i eth0"    
* the third line says that the end of the config is reached so that the entry in the fourth line will be ignored     
   
***
***
## How to dev 

-   Each Pipe Reader shall run in a single Thread. 
-   All pipe-reader obj shall have the same outgoing pipe obj
   
## Function   

### main
The main function can be called either with no or with two parameters.   
If there is no parameter the program will by default create 3 named pipes at "/tmp/pipeRelay< 0-2>   
by using the parameter ["pipe"],[< number >]  the program will create < number> pipes at "/tmp/pipeRelay[0-(number-1)]   
If the parameter is ["ssh"],["name"] the program will use a ssh connection to the clients name. For this purpose the name of the config file
  describes the name of the client the host is supposed to connect to.   
   
### Reader
Pipereader is the reading part of this application. 
Each reader shall run in an single thread 
-Constructor
     
        reader(location, writer, name, message, log)
            string *location    : location of the named pipe               default : "/tmp/pipeRelayOut"
            writer *writer      : outgoing obj (each obj shall have the same)
            string *name        : identifier in log and messages           default : "unnamed reader"
                                         
        
- open()   
    After creating the reader obj the main application has to run the open() method 
    This method creates a named pipe at <location> (and deletes the old file if existent)
    open() blocks until a write app opens the other end of the named pipe
    
- read(char * buff, int size)   
   reads < size > bytes from the ingoing pipe and writes them to the given buffer
 
### Writer
Pipewriter is the writing part of this application.
There is only one writer. 
Each reader thread sends its pcap data to this writer. The methods writer (pcap) and write (byte[], size) are 
protected by an mutex, which prevents simultaneous writing by differned threads 
-   Constructor

        writer(location, name , message, log)
            string *location    : location of the named pipe                default : "/tmp/pipeRelayOut"
            string *name        : identifier in log and messages            default : "unnamed writer"
        
-   open()   
        After creating the writer obj the application has to open the writer
        this method creates a named pipe at <location> (and deletes the old on if existent)
        open() blocks until the reading end of the named pipe gets opend
-   write(char * byte , size)   
        Write a byte array to the pipe
        this method uses mutex to avoid simultaneous writings
        
  

 
### SSH
This application supports an embedded ssh connection to the remote host (using the default ssh program on your client pc).   
For this purpose it is necessary to be able to login passwordless at the remote client by using e.g a keyfile.   
The user has to tell the application  which clients to connect to by an config file.  
Some applications at the remote client must run as root.
To avoid password leaks in config files it is recommended to add the application to the "visudo" list. This enables that the application 
does not request an sudo password.   
Alternatively, you can use an keypass program (in the config file).   
   
The embedded ssh connection has lots of overhead at the beginning. Therefor the run method first checks the 
pcap signature and then reads as long as there is no signature.   
The main usage will be tcpdump. run() will checks for the tcpdump signature and cuts it out.
    
    ssh(host, user, remoteExecute, *outPipe)
    or
    ssh(ReadConfig::entry *inputEntry, *outPipe) 
    
        string host                    :     IP-Address or the host name 
        string user                    :     user name on the remote machine
        string remoteExecute:          :     command that will be executed on the remote machine 
        ReadConfig::entry *inputEntry  :     struct created by the config reader
        writer *outPipe                :     outgoing pipeWriter. The ssh obj send its pcap to this class

* bool open()   
opens the ssh conection    
  
* void run()    
Starts the ssh loop. At first it cuts out all ssh related overhead. Thereafter it sends the pcap-file-header to the outgoing pipe 
and starts reading pcaps

* read(char * buff, int size)   
   reads < size > bytes from the ingoing pipe and writes them to the given buffer
 

