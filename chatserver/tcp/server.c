#include <sys/socket.h>
#include <string.h> //memset
#include <arpa/inet.h> //sockaddr_in and inet_pton
#include <netinet/in.h> //INADDR_ANY
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BACKLOG 5
#define BUFFERSIZE 1024


/*TO DO:
 * add maybe asynchronious processing 
 * add real chat between two users 
 * maybe add some kind of ids for users
 * 
 * */



void error(const char *msg)
{
  perror(msg);
  exit(1);
}

int main(int argc, char *argv[])
{
  //usage serverfilename <port>

  int lsocket, csocket; //listening socket and socket for connected client
  int port;
  struct sockaddr_in serveraddr, clientaddr;
  char buffer[BUFFERSIZE];


  if (argc>1)
    port = atoi(argv[1]);
  else
    port = 8080;


  //structure that represent server's address
  memset(&serveraddr, 0, sizeof(serveraddr));
  serveraddr.sin_family = AF_INET;
  serveraddr.sin_port = htons(port);
  serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);

  //initializing listening socket
  lsocket = socket(AF_INET, SOCK_STREAM, 0);
  if (lsocket<0)
  {
    error("socket creation failed");
  }

  //some cool thing to prevent error on binding "address already in use"
  //dont know how exactly it works but it looks useful so just put it here
  //int optval = 1;
  //setsockopt(lsocket, SOL_SOCKET, SO_REUSEADDR, 
	     //(const void *)&optval , sizeof(int));


  

  //binding it to specific port
  int rc = bind(lsocket, (struct sockaddr*) &serveraddr, sizeof(serveraddr));
  if (rc<0)
    error("binding socket failed");


  if (listen(lsocket, BACKLOG) < 0)
    error("listening start failed");

  printf("Server listening port %hu", port);
  


  for(;;){
    socklen_t clientlen = sizeof(clientaddr);
    csocket = accept(lsocket, (struct sockaddr*)&clientaddr, &clientlen);
    if (csocket < 0)
      error("client connection failed");

    int bytesReaded = read(csocket, buffer, BUFFERSIZE);
    if (bytesReaded > 0)
    {
      printf("Received bytes: %i", bytesReaded);
      printf("Message: %s", buffer);
    }

      close(csocket);
  }  

  close(lsocket);
}






















