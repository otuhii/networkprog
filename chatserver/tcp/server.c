#include <sys/socket.h>
#include <string.h> //memset
#include <arpa/inet.h> //sockaddr_in and inet_pton
#include <netinet/in.h> //INADDR_ANY
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h> 



#define BACKLOG 5
#define BUFFERSIZE 1024


/*TO DO:
 * maybe add a lot of users that can connect, try to make it via different devices 
 * maybe add some kind of ids for users
 *    --i can achieve that by using some structure and maybe sharing not just string but whole structre with name, and message
 * */
void* handleFClient(void* args)
{
  char buffer[BUFFERSIZE];
  int* sockets = (int*)args;
  while(1)
  {
    int bytesReaded = read(sockets[0], buffer, BUFFERSIZE);
    if (bytesReaded > 0)
    {
      printf("Received bytes: %i", bytesReaded);
      printf("Message from first user: %s", buffer);
    }
    write(sockets[1], buffer, BUFFERSIZE);

  }
  close(sockets[0]);
  pthread_exit(NULL);
}


void* handleSClient(void* args)
{
  char buffer[BUFFERSIZE];
  int* sockets = (int*)args;
  while (1)
  {
    int bytesReaded = read(sockets[1], buffer, BUFFERSIZE);
    if (bytesReaded > 0)
    {
      printf("Received bytes: %i", bytesReaded);
      printf("Message from second user: %s", buffer);
    }
    write(sockets[0], buffer, BUFFERSIZE);

  }
  close(sockets[0]);
  pthread_exit(NULL);

}

void error(const char *msg)
{
  perror(msg);
  exit(1);
}


int main(int argc, char *argv[])
{
  //usage serverfilename <port>

  int listeningSocket; //listening socket and socket for connected client
  int cSockets[2];

  int port;
  struct sockaddr_in serveraddr, clientaddr;
  //threads in order to make our clients communicate asynchronically
  pthread_t thread1, thread2;

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
  listeningSocket = socket(AF_INET, SOCK_STREAM, 0);
  if (listeningSocket<0)
  {
    error("socket creation failed");
  }

  //some cool thing to prevent error on binding "address already in use"
  //dont know how exactly it works but it looks useful so just put it here
  int optval = 1;
  setsockopt(listeningSocket, SOL_SOCKET, SO_REUSEADDR, 
	     (const void *)&optval , sizeof(int));

  //binding it to specific port
  int rc = bind(listeningSocket, (struct sockaddr*) &serveraddr, sizeof(serveraddr));
  if (rc<0)
    error("binding socket failed\n");


  if (listen(listeningSocket, BACKLOG) < 0)
    error("listening start failed\n");

  printf("Server listening port %hu\n", port);
  socklen_t clientlen = sizeof(clientaddr);


  cSockets[0] = accept(listeningSocket, (struct sockaddr*)&clientaddr, &clientlen);
  if (cSockets[0] < 0)
    error("first client connection failed\n");

  printf("First client connected succesfull\n");

  cSockets[1] = accept(listeningSocket, (struct sockaddr*)&clientaddr, &clientlen);
  if (cSockets[1]<0)
    error("second client connection failed\n");

  printf("Second client connected succesfully\n");

  //after succesfull connection of each client we can start chatting yay

  pthread_create(&thread1, NULL, handleFClient, (void*)cSockets);
  pthread_create(&thread2, NULL, handleSClient, (void*)cSockets); 

  pthread_detach(thread1);
  pthread_detach(thread2);
  
  for(;;){}


  close(listeningSocket);
}



