# TCP_Server_Client--Multi-Connections
 Server that accepts TCP protocol connections, and runs a command and response method.
    handling multiple connections at the same time.
        (C - Linux - Multithreading)

Execution and User Manual instructions:
Compile the program used "make" command (to execute the make file).

Execute program command-line, 2 options:  
./server [DataSavingSystem] [port] [path]  
or  
./server --help  

while:  
[DataSavingSystem] - "tree" or "txt".  
[port] - any integer port number.  
[path] - path for saving the output. (does not end with "/")  

Execute example::  
./server tree 8080 .  

For helping, we execute the program with the "--help" parmeter.

#Note   
when used the "txt" parameter, a txt file will created in  
your chosen path with the file name: "saveSys.txt".  

-----------------------------------------

After we set up the server:  
open another terminal and run the command-line:  
nc [IP] [PORT]  

while:  
[IP] - ipv4 address of the computer let the server run.  
[PORT] - the port the server is listening to.  

-----------------------------------------

When we connect to the server with the "tree" parameter we note that  
when we create a path its make the folders name with underline  
(e.g. "foldername_"),  
so we can distinguish a folder from a file.  
but the user does not refer to the underline when writing  
the commands to the server.  

-----------------------------------------

A service file is also attached.  
The usage and explanations are found in the file.  
In file - username must be changed from "avivye" to yours and also the path.  


=========================================
Server Command Rules - Important!  

Options commands:  
    1 "." Track "" ID  
    2 ”.” Track  
    3 ”+“  
    4 ”*“  
    5 ”-“  
Comments:  
    1. Empty response. The program will execute the insertion of the given value as an in-track ID.  
    2. The response is the last value placed in the track.  
    3. Empty response - If a track has a numeric value (ie only digits), the program will increase it In one.  
    4. The response is the sum of all numeric values ​​that have been placed in the paths that the given path is valid their. 0 if none.  
    5. The last value placed in the path received from the concatenation of the given path, "." and the value placed in the path.  

    In the event that the value placed in the path should be considered, but nothing has been placed in the path, the response will be empty.  

The values ​​will be kept from running on a storage device. Two data retention systems must be written:  
    1. All data in one text file.  
    2. The data will be stored in the directory tree, each entry in a separate file.  

The program will be capable of handling multiple connections at the same time.  

=========================================
