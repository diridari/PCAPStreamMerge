# PCAP-Relay   
## Goal   
This application has the goal to combine the traffic of several Network devices to one outgoing date stream. 
This allows you to simplify the analytics and bug finding of mesh-networks.

You may use this application if you develop IEE 802.15.4 (IoT) devices or other kinds of Mesh networks,
 where it is not possible to capture the whole network by one single sniffing-device.  

The Host request the tcpdump(or a other pcap source) from the Clients read and combine it.
There are some basic filters witch allows to suppress double received packages e.g. if one datapackage has been captured by two 
sniffers.
