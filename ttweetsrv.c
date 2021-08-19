/* 
 * Author: Conner Bluck
 * Email: cbluck3@gatech.edu
 * Class: CS3251 Computer Networks 1
 * Date: 06/14/2021
 * Assignment: Trivial Twitter application (Programming Assignment 1)
 * 
 * some of this code was used or referenced from an echo server template from carnegie mellon university
 * reference: http://www.cs.cmu.edu/afs/cs/academic/class/15213-f00/www/class24code/echoserver.c
 * 
 * ttweetsrv.c - A simple TCP connection-based server. The server listens
    for a request from the client and sends back the appropriate message
 *  
 * command to start running server: ./ttweetsrv <ServerPort>
 */



#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
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

/* client handling function */
void HandleClient(int clientSock, char *ttweetMessage);

int main(int argc, char *argv[]) {
  int serverSock;                 //listening socket     
  int clientSock;                 //connection socket
  int portno;                     //port number
  int clientLen;                  //byte size of clients address
  char ttweetMessage[150];         //message to be uploaded
  struct sockaddr_in serverAddr;  //server address
  struct sockaddr_in clientAddr;  //client address

  /* initialize message to empty */
  ttweetMessage[0] = '\0';

  /* test for correct number of arguments */
  if (argc != 2){
    printf("Error: Incorrect arguemnt usage\n");
    printf("Correct Usage: %s <Server Port>\n", argv[0]);
    exit(1);
  }

  portno = atoi(argv[1]);

  /* socket: create a socket */
  serverSock = socket(AF_INET, SOCK_STREAM, 0);
  if (serverSock < 0) {
    error("ERROR opening socket");
  }

  /* build the server's internet address */
  memset(&serverAddr, 0, sizeof(serverAddr));
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
  serverAddr.sin_port = htons((unsigned short)portno);

  /* bind: associate the listening socket with a port */
  if (bind(serverSock, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) < 0) {
    error("ERROR on binding");
  }

  /* listen: make it a listening socket ready to accept connection requests */
  if (listen(serverSock, 5) < 0) { /* allow 5 requests to queue up */
    error("ERROR on listen");
  }  

  /* main loop: wait for a connection request */
  clientLen = sizeof(clientAddr);
  while (1) {
      
    /* accept: wait for a connection request */
    clientSock = accept(serverSock, (struct sockaddr *) &clientAddr, &clientLen);
    if (clientSock < 0) {
      error("ERROR on accept");
    }

    printf("Handling client %s\n", inet_ntoa(clientAddr.sin_addr));

    HandleClient(clientSock, ttweetMessage);
  }
}

void HandleClient(int clientSock, char *ttweetMessage) {
  int n;                                              //message byte size                           
  char ttweetBuffer[BUFSIZE];                         //buffer for message
  char emptyMessage[] = "EMPTY Message";              //message if nothing in ttweetMessage
  char uploadSuccessMessage[] = "Upload Successful";  //successful upload message

  /* recieve message in ttweetbuffer */
  memset(ttweetBuffer, 0, BUFSIZE);
  n = read(clientSock, ttweetBuffer, BUFSIZE);
  if (n < 0) {
    error("ERROR reading from socket");
  }

  /* if nothing in buffer and nothing in ttweetmessage, send empty message */
  if (strlen(ttweetBuffer) == 0) {
    if (ttweetMessage[0] == '\0') {
      n = write(clientSock, emptyMessage, strlen(emptyMessage));
      if (n != strlen(emptyMessage)) {
        error("ERROR reading from socket");
      }
    /* if nothing in buffer, send ttweetmessage */
    } else {
      n = write(clientSock, ttweetMessage, strlen(ttweetMessage));
      if (n != strlen(ttweetMessage)) {
        error("ERROR reading from socket");
      }
    }
  /* send successful upload message */
  } else {
    strcpy(ttweetMessage, ttweetBuffer);
    n = write(clientSock, uploadSuccessMessage, strlen(uploadSuccessMessage));
    if (n != strlen(uploadSuccessMessage)) {
      error("ERROR reading from socket");
    }
  }

  close(clientSock);
}
