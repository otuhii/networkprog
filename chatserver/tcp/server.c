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
 * maybe add a lot of users that can connect, try to make it via different devices 
 * maybe add some kind of ids for users
 *    --i can achieve that by using some structure and maybe sharing not just string but whole structre with name, and message
 * */



void error(const char *msg)
{
  perror(msg);
  exit(1);
}

int main(int argc, char *argv[])
{
  //usage serverfilename <port>

  int lSocket, firstcliSocket, secondcliSocket; //listening socket and socket for connected client
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
  lSocket = socket(AF_INET, SOCK_STREAM, 0);
  if (lSocket<0)
  {
    error("socket creation failed");
  }

  //some cool thing to prevent error on binding "address already in use"
  //dont know how exactly it works but it looks useful so just put it here
  int optval = 1;
  setsockopt(lSocket, SOL_SOCKET, SO_REUSEADDR, 
	     (const void *)&optval , sizeof(int));

  //binding it to specific port
  int rc = bind(lSocket, (struct sockaddr*) &serveraddr, sizeof(serveraddr));
  if (rc<0)
    error("binding socket failed");


  if (listen(lSocket, BACKLOG) < 0)
    error("listening start failed");

  printf("Server listening port %hu", port);
  
  socklen_t clientlen = sizeof(clientaddr);


  firstcliSocket = accept(lSocket, (struct sockaddr*)&clientaddr, &clientlen);
  if (firstcliSocket < 0)
    error("first client connection failed");

  printf("First client connected succesfully");

  secondcliSocket = accept(lSocket, (struct sockaddr*)&clientaddr, &clientlen);
  if (secondcliSocket<0)
    error("second client connection failed");

  printf("Second client connected succesfully");

  //after succesfull connection of each client we can start chatting yay


  for(;;){

    int bytesReaded = read(firstcliSocket, buffer, BUFFERSIZE);
    if (bytesReaded > 0)
    {
      printf("Received bytes: %i", bytesReaded);
      printf("Message from first user: %s", buffer);
    }
    write(secondcliSocket, buffer, BUFFERSIZE);

    
    bytesReaded = read(secondcliSocket, buffer, BUFFERSIZE);
    if (bytesReaded >0)
    {
      printf("Received bytes: %i", bytesReaded);
      printf("Message from second user: %s", buffer);
    }
    write(firstcliSocket, buffer, BUFFERSIZE);

  }  

  close(lSocket);
}

















