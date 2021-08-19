 /* 
 * Author: Conner Bluck
 * Email: cbluck3@gatech.edu
 * Class: CS3251 Computer Networks 1
 * Date: 06/14/2021
 * Assignment: Trivial Twitter application (Programming Assignment 1)
 * 
 * some of this code was used or referenced from an echo clinet template from carnegie mellon university
 * reference: http://www.cs.cmu.edu/afs/cs/academic/class/15213-f00/www/class24code/echoclient.c
 * 
 * 
 * ttweetcl.c - A simple TCP connection-based client. The client can upload a message to the 
    server or download a message from the server 
 *
 * command to download from server: ./ttweetcl -d <ServerIP> <ServerPort>
 * command to upload to server: ./ttweetcl -u <ServerIP> <ServerPort> "message"
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h> 

#define BUFSIZE 1024

/* 
 * error - wrapper for perror
 */
void error(char *errorMessage) {
  perror(errorMessage);
  exit(1);
}

int main(int argc, char *argv[]) {
  int errorFlag = 0;                  //variable to flag errors            
  int uploadFlag = 0;                 //variable to flag uploads               
  int sock;                           //socket
  int portno;                         //port number
  int ttweetStringLen;                //ttweet string length
  int n;                              //number of bytes read from incoming messages                                                             
  char *srvIP;                        //server IP
  char *ttweetString;                 //string to be uploaded to server
  char ttweetBuffer[BUFSIZE];         //buffer for ttweet string
  struct sockaddr_in ttweetServAddr;  //ttweet server address

  /* tests for correct number of arguments */
  if ((argc < 4) || (argc > 5)) {
    errorFlag = 1;
  }

  /* argument 2 is server IP and argument 3 is port number*/
  srvIP = argv[2];
  portno = atoi(argv[3]);

  /* tests for upload/download and correct arguments */
  if (strcmp(argv[1], "-u") == 0) {                 
    if (argc == 5){                                
      ttweetString = argv[4];
      ttweetStringLen = strlen(ttweetString);
      if (ttweetStringLen < 1) {
        errorFlag = 1;
      }
      if (ttweetStringLen > 150) {
        printf("Error: maximum tweet length is 150\n");
        exit(1);
      }
      uploadFlag = 1;
    } else {
      errorFlag = 1;
    }
  } else if (strcmp(argv[1], "-d") == 0) {
    if (argc != 4) {
      errorFlag = 1; 
    }
  } else {
    errorFlag = 1;
  }

  /* if something in the arguments are wrong */
  if (errorFlag) { 
    fprintf(stderr, "Correct Usage for upload: %s -u <ServerIP> <ServerPort> \"message\"\n", argv[0]);
    fprintf(stderr, "Correct Usage for download: %s -d <ServerIP> <ServerPort>\n", argv[0]);
    exit(1);
  }

  /* socket: create the socket */
  sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock < 0) {
    error("ERROR opening socket");
  }

  /* build the server's Internet address */
  memset(&ttweetServAddr, 0, sizeof(ttweetServAddr));
  ttweetServAddr.sin_family = AF_INET;
  ttweetServAddr.sin_addr.s_addr = inet_addr(srvIP);
  ttweetServAddr.sin_port = htons(portno);

  /* connect: create a connection with the server */
  if (connect(sock, (struct sockaddr *) &ttweetServAddr, sizeof(ttweetServAddr)) < 0) {
    printf("Error Connecting: Server Not Found\n");
    exit(1);
  }

  /* if client wants to upload message, upload message */
  if (uploadFlag){
    if (write(sock, ttweetString, ttweetStringLen) != ttweetStringLen)
        error("ERROR writing to socket");
  }

  /* shutdown socket sending */
  shutdown(sock, SHUT_WR);

  /* recieve message from server */
  memset(ttweetBuffer, 0, BUFSIZE);
  n = read(sock, ttweetBuffer, BUFSIZE);
  if (n < 0) {
    error("ERROR reading from socket");
  }
  printf("%s", ttweetBuffer);
  printf("\n");

  close(sock);
  exit(1);
}
