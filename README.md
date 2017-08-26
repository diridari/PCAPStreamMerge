# Pipe-Relay

## Goals
This Application has the goal to combine the data stream of of several ingoing data-streams to one outgoing data-stream. 
To do this it's links both several ingoing named-pipes or SSH data-streams to one outgoing named pipe.
This is necessary if you have
  - several remote connections( SSH, serial ...) where the ingoing traffic shall be combined
  - several apps whose transmitted data shall be combined to one pipe

This application(is the base of Pcap-Relay ) has been written to enable sniffing with several remote Clients. 
Each Client sends its tcpdump via ssh to a named pipe at the host system.
All named pipes are joined to one outgoing pipe witch can be readLine by wireshark   
   
Also is it possible to an embedded ssh connection(uses the cli ssh command of the host system), here you can 
connect to several remote clients and transfer an remote command witch will be executed and its data send to the host-computer.
***
## How to use
start the programm from the cli. 
you can user these parameter:
            
               -ssh     <configFile>
               -pipe    <numberOf>
               -l       <outPutLocation>        	default : /tmp/pipeRelay 
               -log     <loglevel>                  how verbose this application is default:0 max:4   
               <no paramether>                  	2 named pipes


            
   < configFile > is the name and/or the location of the file witch describe the ssh connection.  
    
   < number of > is the number of named pipe witch the programm open.     
   The named pipes are stored at "/tmp/pipeRelay< number >"
   if there is no parameter the application create per default 3 named-pipes.    
   
   "encap"sulation: linux encasulat network packages from a unknown source. Foren6 is not able to handle sutch Packages
   the solution in this case is to remove this encapsulation Header and change the pcap-linkylaer to a 6lowpan one.
   By default this feature is enabled.
   
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
           myServer  sudo tcpdump -i eth0
           
           8.8.8.8 google please hack that for me :)   
           
 * the first line says that the host ist "127.0.0.1" the user is "LinuxUser" and the command is "sudo tcpdump -i eth0"   
 * the second line says that the host ist "myServer" the user is empty(2 spaces)  and the command is "tcpdump -i eth0"    
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
 

