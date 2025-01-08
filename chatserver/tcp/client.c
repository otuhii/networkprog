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
#define FILEBUFFERSIZE 8192


//structure for authorized messaging 
typedef struct messageBlock{
  char username[NAMESIZE];
  char messageBuffer[BUFFERSIZE];
} messageBlock;


//struct for sending and receiving file
typedef struct threadFileArgs{
  char filename[NAMESIZE];
  int mode; //1 - receiving, 2 - sending
  int sSock;
} threadFileArgs;


//struct for thread function listenServer
typedef struct threadListenArgs{
  int ftSock;
  int sSock;
  pthread_t ftThread;
} threadListenArgs;

void error(const char* msg){
  perror(msg);
  exit(1);
}


void sendfile(int sSock, char* filename)
{
    int n;
    FILE *fp;
    char sendbuffer[FILEBUFFERSIZE];
    
    ///////////////////////////////////////////////////
    //      send filename length and filename        //
    ///////////////////////////////////////////////////
    uint32_t filename_len = strlen(filename);
    if(send(sSock, &filename_len, sizeof(uint32_t), 0) < 0)
        error("error in sending filename length");
    if(send(sSock, filename, filename_len, 0) < 0)
        error("error in sending filename");
    ///////////////////////////////////////////////////
    
    fp = fopen(filename, "rb");
    if(fp == NULL)
      error("file opening failed");

    while((n = fread(sendbuffer, sizeof(char), FILEBUFFERSIZE, fp)) > 0)
    {
        if(send(sSock, sendbuffer, n, 0) < 0)
          error("error in sending file");
        bzero(sendbuffer, FILEBUFFERSIZE);
    }
    fclose(fp);
}



void recvfile(int sSock)
{
    int n;
    FILE *fp;
    char recvbuffer[FILEBUFFERSIZE];
    

    /////////////////////////////////////////////////////////
    uint32_t filename_len;
    if(recv(sSock, &filename_len, sizeof(uint32_t), 0) <= 0)
      error("[-]Error in receiving filename length");
    
    char *filename = malloc(filename_len + 1);
    if(filename == NULL)
      error("[-]Error allocating memory");
    
    if(recv(sSock, filename, filename_len, 0) <= 0)
    {
        free(filename);
        error("error in receiving filename");
    }
    filename[filename_len] = '\0';   
    
    fp = fopen(filename, "wb");
    if(fp == NULL)
    {
        free(filename);
        error("error in creating file");
    }

    while(1)
    {
        n = recv(sSock, recvbuffer, FILEBUFFERSIZE, 0);
        if(n <= 0)
            break;
        fwrite(recvbuffer, sizeof(char), n, fp);
        bzero(recvbuffer, FILEBUFFERSIZE);
    }

    fclose(fp);
    free(filename);
}


int connectSocket(const char* ip, int port) 
{
  int sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock < 0) 
    return -1;

  struct sockaddr_in server_addr;
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(port);
  server_addr.sin_addr.s_addr = inet_addr(ip);

  if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) 
    return -1;

  return sock;
}


void* fileTransfer(void*arg)
{
  threadFileArgs *args = (threadFileArgs *)arg;
  int mode = args->mode;
  int sSock = args->sSock;
  char *filename = args->filename;

  if (mode==1)
    recvfile(sSock);
  else if (mode ==2)
      sendfile(sSock, filename);
  else
    error("invalid mode variable initialization");

  free(args);
  pthread_exit(NULL);
      
}


void* listenServer(void* arg)
{
  threadListenArgs *args = (threadListenArgs*)arg;
  messageBlock message;
  while(connection)
  {
    int bytesReaded = recv(args->sSock, &message, sizeof(message), 0);
  
    if(bytesReaded)
    {
      if(strcmp(message.messageBuffer, "file") == 0)
      {
        threadFileArgs* info = malloc(sizeof(threadFileArgs));
        if (info == NULL) error("Memory allocation failed");
        info->mode = 1;
        info->sSock = args->ftSock;
        pthread_create(&args->ftThread, NULL, fileTransfer, info);
      
        pthread_join(args->ftThread, NULL);
      }
      else 
      {
        message.messageBuffer[BUFFERSIZE-1] = '\0'; 

        // Move the cursor to the beginning of the line and clear the input prompt
        printf("\r\033[K"); // '\r' moves to the beginning, '\033[K' clears the line
        printf("Message from %s: %s\n", message.username, message.messageBuffer);
        printf("Enter msg to send: ");
        fflush(stdout);
      }
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




void startMessaging(int sSock,int ftSock,pthread_t ftThread, messageBlock* message)
{
  while(connection)
  {
    printf("Enter msg to send: ");
    bzero(message->messageBuffer, BUFFERSIZE);
    fgets(message->messageBuffer, BUFFERSIZE, stdin);
    message->messageBuffer[strcspn(message->messageBuffer, "\n")] = 0;
    
    if(send(sSock, message, sizeof(*message), 0) == -1) 
        error(" Error in sending message");
    
    if(strcmp(message->messageBuffer, "file") == 0)
    {
      threadFileArgs* args = malloc(sizeof(threadFileArgs));
      if (args == NULL) error("Memory allocation failed");

      printf("\nenter file to send: ");
      bzero(args->filename, NAMESIZE);
      fgets(args->filename, NAMESIZE, stdin);
      args->sSock = ftSock;
      args->mode = 2;
      pthread_create(&ftThread, NULL, fileTransfer, args);
    }

  }
}


int main(int argc, char* argv[])
{
  //usage clientprogram <hostname> <port>

  int csocket, ftsocket;//idk why its csocket but im really lazy so would not change that, ft - file transfer
  
  pthread_t listeningThread, filetransferThread;  

  messageBlock umessage;
  

  int port;
  char *server_ip;
  if (argc != 3)
    error("wrong arguments! \nusage clientprogram <hostname> <port>");
  else
  {
     port = atoi(argv[2]);
     server_ip = argv[1];
  }
  
  csocket = connectSocket(server_ip, port); 
  if (csocket==-1)
    error("can't create socket");
  ftsocket = connectSocket(server_ip, port+1);
  if (ftsocket == -1)
    error("can't create file transfering socket");

  connection = 1;

  
  threadListenArgs* info = malloc(sizeof(threadListenArgs));
  if (info == NULL) error("Memory allocation failed");
  info.ftSock = ftsocket;
  info.sSock = csocket;
  info.ftThread = filetransferThread;

  //setting username for structure
  printf("enter your username: ");
  bzero(umessage.username, NAMESIZE);
  fgets(umessage.username, NAMESIZE, stdin);
  umessage.username[strcspn(umessage.username, "\n")] = 0;
  
  printf("connected to server %s on port %d", server_ip, port);


  pthread_create(&listeningThread, NULL, listenServer, (void*)&info);
  startMessaging(csocket,ftsocket,filetransferThread, &umessage); 

  close(csocket);
  return 0;
}
