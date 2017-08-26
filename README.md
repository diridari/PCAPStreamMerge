# PCAP-Relay   
## Goal   
This application has the goal to combine the traffic of several Network devices and combine them to one date Stream. This allows you to simplify the analytics and bug finding.

You may use this application if you develop IEE 802.15.4 (IoT) devices or other kinds of Mesh networks, where it is not possible to capture the whole Network.  

This application has been written to enable sniffing a Mesh-Net where it is not possible to capture the whole network with just 
one single Sniffer. 
The Host request the tcpdump(or a other pcap source) from the Clients and 

This application is based on Pipe-Relay wich was written to combine several ingoing pipes to one outgoing pipe.

***
## How to use
start the programm from the cli. 
you can user these parameter:
            
                    -ssh <configfileName/location>      read config file  
                    -pipe <nuberOf>                     open <numberOf named pipes und store them at <location>
                    -l <location>                       change the default location to <location> and apend In and Out<n>
                    -ch <channel>                       set the Channel (default: 26) just used by -ssh
                    -log <loglevel>                     set loglevel default : 0
                    -history <time>                     enables the pcap history
                    -encap                              set to disable the removing of the Linux Cooked Encapsulation 
                    -noRemoteSetup                      disable the setup of the ssh device before executing the remote command


            
   < configFile > is the name and/or the location of the file witch describe the ssh connection (see: "how it work's::ssh").
   
   < number of > is the number of named pipe witch the programm open.   
          The named pipes are stored at "/tmp/pipeRelay< number >" or if given at the with -l provided location   
          If there is no parameter the application create per default 3 named-pipes
          
   "encap"sulation: linux encasulat network packages from a unknown source. Foren6 is not able to handle sutch Packages
   the solution in this case is to remove this encapsulation Header and change the pcap-linkylaer to a 6lowpan one.
   By default this feature is enabled.
   
   If you want to connect to a remote device via SSH it recommended to enable a password less connection with public/private key as authentication. 

***
## How it work's
The application open one outgoing pipe which get filled with the combined ingoing traffic (the program blocks until the pipe has a reading end).    
Then for each target device(named pipe at the local machine or remote with SSH) the program opens one tunnel.   
* if the device is a named pipe: the content  be pcap
* if the device is a SSH connection: the application connect to the devices defined in the config file.
### Config-File
If the application shall run in ssh mod the user has to define an config file where the app can read witch clients to connect,
and witch applications shall run at the remote client.   
The config file has following definitions:   
- each entry stands in its own line
- an space(' ') separate the entry-parts
- an empty line ore the EOF define the end of the config file
- there are max 200 chars per line allowed   

#### Syntax:   
* < hostname or ip> < space> < username> < space> < remote command>   

* < hostname or ip> < space> < username> < space> < remote command>   

* ...   
   
e.g   

          127.0.0.1 linuxUser sudo tcpdump -i eth0
          myServer  sudo tcpdump -i eth0  // invalid          
          8.8.8.8 google please hack that for me :)   
          
* the first line says that the host ist "127.0.0.1" the user is "LinuxUser" and the command is "sudo tcpdump -i eth0"   
* the second line says that the host ist "myServer" the user is empty(2 spaces)   and the command is "tcpdump -i eth0"    
* the third line says that the end of the config is reached so that the entry in the fourth line will be ignored     
   

## How to user(dev) 

-   Each Pipe Reader shall run in a single Thread. 
-   All pipe-reader obj shall have the same outgoing pipe obj
   
## Function   

### main
The main function can be called with non or with 2 parameter.   
If there is no parameter the program will create by default 3 named pipes at "/tmp/pipeRelay< 0-2>   
by using the parameter ["pipe"],[< number >]  the program will create < number> pipes at "/tmp/pipeRelay[0-(number-1)]   
If the parameter is ["ssh"],["name"] the program use an ssh connection to the clients name is the name of the config file
  witch describe to witch clients the host shall connect.   
   
### Reader
Pipereader is the reading part of this application. 
Each reader shall run in an singe thread 
-Constructor
     
        reader(location, writer, name, message, log)
            string *location    : location of the named pipe               default : "/tmp/pipeRelayOut"
            writer *writer      : outgoing obj (each obj shall have the same)
            string *name        : identifier in log and messages           default : "unnamed reader"
                                         
        
- open()   
    After creating the reader obj the main application has to run the open() method 
    This method create an named pipe at <location> (and delete the an old file if there is one)
    open() blocks until an write app opens the other end of the named pipe
    
- read(char * buff, int size)   
   reads < size > bytes from the ingoing pipe and write them to the given buffer
 
### Writer
Pipewriter is the writing part of this application.
There is one writer. 
Each reader thread sends its pcap data to this writer. The methods writer (pcap) and write (byte[], size) are protected by an mutex witch prevent simultaneous writing by differned threads 
-   Constructor

        writer(location, name , message, log)
            string *location    : location of the named pipe                default : "/tmp/pipeRelayOut"
            string *name        : identifier in log and messages            default : "unnamed writer"
        
-   open()   
        After creation the writer obj the applications has to open the writer
        this method creat an named pipe at <location> (and delete an old on if ther is on)
        open() blocks until the reading end of the named-pipe gets opend
-   write(char * byte , size)   
        Write an byte array to the pipe
        this method uses mutex to avoid simultaneous writings
        
  

 
### SSH
This application supports an embedded ssh connection to the remote host (using the default ssh program on your client pc).   
To do this is is necessary that it is possible to login passwordless at the remote client by using e.g an keyfile.   
The user has to tell the application by an config file witch clients to connect.  
Some Application at the remote client musts run as root.
To avoid password leaks in config files i recommend to add the application to the "visudo" list to enable that this application 
does not request an sudo password.   
Alternative you can use an keypass program(in the config file).   
   
The embedded ssh connection has at it beginning a lot of overhead so the run method checks at first if the 
pcap Signature and read as long as there has not been the signature.   
The main usage will be tcpdump, so run() will check if the next is the tcpdump signature and cut it out.
    
    ssh(host, user, remoteExecute, *outPipe)
    or
    ssh(ReadConfig::entry *inputEntry, *outPipe) 
    
        string host                    :     IP-Address or the host name 
        string user                    :     user name on the remote machine
        string remoteExecute:          :     commend that will be executed on the remote machine
        ReadConfig::entry *inputEntry  :     strct created by the config reader
        writer *outPipe                :     outgoing pipeWriter. The ssh obj send its pcap to this class

* bool open()   
opens the ssh conection    
  
* void run()    
Stats the ssh loop. At first it cut out all ssh related overhead. After that it sends the pcap-file-header to to outgoing pipe 
and stats reading pcaps

* read(char * buff, int size)   
   reads < size > bytes from the ingoing pipe and write them to the given buffer
 

