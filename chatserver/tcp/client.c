#include <sys/socket.h>
#include <arpa/inet.h> 
#include <stdio.h>  
#include <stdlib.h>  
#include <unistd.h>  
#include <string.h> 

#define BUFFERSIZE 1024



void error(const char* msg){
  perror(msg);
  exit(1);
}

int main(int argc, char* argv[])
{
  //usage clientprogram <hostname> <port>

  int csocket, hsocket;
  struct sockaddr_in serveraddr;
  const char *server_ip = argv[1];
  int port = atoi(argv[2]);
  char buffer[BUFFERSIZE];


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

  printf("connected to server %s on port %");
  
  while(1)
  {

    printf("Enter msg to send: ");
    bzero(buffer, BUFFERSIZE);
    fgets(buffer, BUFFERSIZE, stdin);

    int n = write(csocket, buffer, sizeof(buffer));
    if (n < 0)
      error("could not write to socket");
    
    if(read(csocket, buffer, sizeof(buffer)))
    {
      printf("Message from your friend: %s", buffer);
    }


  }

  return 0;
}
