#include <sys/socket.h>
#include <arpa/inet.h> 
#include <stdio.h>  
#include <stdlib.h>  
#include <unistd.h>  
#include <string.h> 
#include <pthread.h>

volatile int connection = 0;

#define BUFFERSIZE 1024

void* listenServer(void* arg)
{

  int csocket = *((int*)arg); 
  char buffer[BUFFERSIZE];
  while(connection)
  {
    int bytesReaded = read(csocket, buffer, sizeof(buffer));
    if(bytesReaded)
    {
      buffer[bytesReaded] = '\0'; // Null-terminate the string

      // Move the cursor to the beginning of the line and clear the input prompt
      printf("\r\033[K"); // '\r' moves to the beginning, '\033[K' clears the line

      // Print the message from the server
      printf("Message from your friend: %s\n", buffer);
            
      // Reprint the input prompt for the user
      printf("Enter msg to send: ");
      fflush(stdout); 
    }
    else if (bytesReaded==0){
      printf("Your friend disconnecte(((\n");
      connection=0;
      break;
    }
    else {
      printf("error occured!! \n");
      connection=0;
      break;
    }
  }
}


void error(const char* msg){
  perror(msg);
  exit(1);
}

int main(int argc, char* argv[])
{
  //usage clientprogram <hostname> <port>

  int csocket;
  struct sockaddr_in serveraddr;
  const char *server_ip = argv[1];
  int port = atoi(argv[2]);
  char buffer[BUFFERSIZE];
  
  pthread_t listeningThread;  


  if (argc != 3)
    error("wrong arguments! \nusage clientprogram <hostname> <port>");

  csocket = socket(AF_INET, SOCK_STREAM, 0);
  if (csocket<0)
    error("user socket creation failed");

  


  //server address structure init
  memset(&serveraddr, 0, sizeof(serveraddr)); 
  serveraddr.sin_family = AF_INET;
  serveraddr.sin_port = htons(port); 


  if (inet_pton(AF_INET, server_ip, &serveraddr.sin_addr) <= 0) 
      error("invalid server IP address");
    

  int rc = connect(csocket, (struct sockaddr *) &serveraddr, sizeof(serveraddr));
  if (rc < 0) 
      error("connection to server failed");

  connection = 1;
  printf("connected to server %s on port %d");


  pthread_create(&listeningThread, NULL, listenServer, (void*)&csocket);
  while(connection)
  {
    printf("Enter msg to send: ");
    bzero(buffer, BUFFERSIZE);
    fgets(buffer, BUFFERSIZE, stdin);

    int n = write(csocket, buffer, sizeof(buffer));
    if (n < 0)
      error("could not write to socket");
    
  }
  close(csocket);
  return 0;
}
