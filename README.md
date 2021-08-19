# ttweet
Author: Conner Bluck

Email: cbluck3@gatech.edu

Class: CS3251 Computer Networks 1

Date: 06/14/2021

Assignment: Trivial Twitter application (Programming Assignment 1)


Files:

ttweetsrv.c - A simple TCP connection-based server. The server listens for a request from the client and sends back the appropriate message 
ttweetcl.c - A simple TCP connection-based client. The client can upload a message to the server or download a message from the server 
sample.txt - A sample output for the provided test scenario

Some of the code in ttweetsrv.c was used or referenced from an echo server template from CMU: http://www.cs.cmu.edu/afs/cs/academic/class/15213-f00/www/class24code/echoserver.c

Some of the code in ttweetcl.c was used or referenced from an echo client template from CMU: http://www.cs.cmu.edu/afs/cs/academic/class/15213-f00/www/class24code/echoclient.c


commands to compile and create an executable for server and client:

gcc ttweetsrv.c -o ttweetsrv

gcc ttweetcl.c -o ttweetcl


instructions for running the programs:

server: ./ttweetsrv <ServerPort>
  
client download: ./ttweetcl -d <ServerIP> <ServerPort>
  
client upload: ./ttweetcl -u <ServerIP> <ServerPort> "message"
  

Sample output in in Sample.txt

Protocol Description:
When the client has an error in the arguments the client recieves these error messages: Correct Usage for upload: ./c -u <ServerIP> <ServerPort> "message", Correct Usage for download: ./c -d <ServerIP> <ServerPort>
  
When a client connects to the server, the server will output: Handling client <IP>
  
When the client tries to upload a message >150 characters, they will get an error and the message will not be uploaded
  
The client can upload any ascii text message 150 characters or less.
  
When the client requests to download from the server, the server will respond by sending the previous uploaded message or "EMPTY message" if no message has been uploaded.




