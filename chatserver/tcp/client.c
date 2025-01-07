#include <sys/socket.h>
#include <arpa/inet.h> 
#include <stdio.h>  
#include <stdlib.h>  
#include <unistd.h>  
#include <string.h> 
#include <pthread.h>

volatile int connection = 0;

#define BUFFERSIZE 1024
#define NAMESIZE 64

//structure for authorized messaging 
typedef struct messageBlock{
  char username[NAMESIZE];
  char messageBuffer[BUFFERSIZE];
} messageBlock;


void error(const char* msg){
  perror(msg);
  exit(1);
}


void* listenServer(void* arg)
{

  int csocket = *((int*)arg); 
  messageBlock message;
  while(connection)
  {
    int bytesReaded = recv(csocket, &message, sizeof(message), 0);
    if(bytesReaded)
    {
      message.messageBuffer[BUFFERSIZE-1] = '\0'; 

      // Move the cursor to the beginning of the line and clear the input prompt
      printf("\r\033[K"); // '\r' moves to the beginning, '\033[K' clears the line
      printf("Message from %s: %s\n", message.username, message.messageBuffer);
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


void startMessaging(int sSock, messageBlock* message)
{
  while(connection)
  {
    printf("Enter msg to send: ");
    bzero(message->messageBuffer, BUFFERSIZE);
    fgets(message->messageBuffer, BUFFERSIZE, stdin);
    
    message->messageBuffer[strcspn(message->messageBuffer, "\n")] = 0;

    if(send(sSock, message, sizeof(*message), 0) == -1) {
        error(" Error in sending message");
    }
  }
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

  messageBlock umessage;

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

  //setting username for structure
  printf("enter your username: ");
  bzero(umessage.username, NAMESIZE);
  fgets(umessage.username, NAMESIZE, stdin);
  umessage.username[strcspn(umessage.username, "\n")] = 0;
  
  printf("connected to server %s on port %d", server_ip, port);


  pthread_create(&listeningThread, NULL, listenServer, (void*)&csocket);
  startMessaging(csocket, &umessage); 

  close(csocket);
  return 0;
}
