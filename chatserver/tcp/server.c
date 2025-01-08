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
#define NAMESIZE 64
#define CLIENTSN 2
#define FILEBUFFERSIZE 8192

//to do:
//  -add multiple users handling
//  -add some checking after receiving file, maybe hash(sha or some shit, would be cool if i can implement it by myself)
// !!!!there is bug so  i can send only one file and then it just freezes and does not allow me send even message: it would be cool to fix that but i don't really care




typedef struct messageBlock{
  char username[NAMESIZE];
  char messageBuffer[BUFFERSIZE];
} messageBlock;


typedef struct serverState {
  int clientSockets[CLIENTSN];
  int ftSockets[CLIENTSN];
  int connectedClients;
  pthread_mutex_t mutex;
} serverState;

serverState server = {
  .connectedClients = 0
};

void error(const char *msg)
{
  perror(msg);
  exit(1);
}


int serverSocketSetup(int port)
{
  int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
  if (serverSocket<0)
    error("creation server socket failed");

  struct sockaddr_in serverAddr;
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_addr.s_addr = INADDR_ANY;
  serverAddr.sin_port = htons(port);

  int opt = 1;
  if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
    error("setsockopt failed");
  if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0)
    error("sever socket binding failed");
  if (listen(serverSocket, 2) < 0)
    error("listening failed");

  return serverSocket;
}


void forwardMessage(messageBlock* message, int senderSocket)
{
  pthread_mutex_lock(&server.mutex);
  for (int i = 0; i < CLIENTSN; i++)
  {
    if (server.clientSockets[i] != senderSocket && server.clientSockets[i]!=0)
    {
      if (send(server.clientSockets[i],message,sizeof(messageBlock), 0) < 0)
      {
        perror("error forwarding message");
      }
    }
  }
  pthread_mutex_unlock(&server.mutex);
}


void handleFileTransfer(int senderftSocket, int receiverftSocket)
{
  uint32_t filename_len;
  char buffer[FILEBUFFERSIZE];
  int n;

  //filename operations
  char filename[NAMESIZE];
  if (recv(senderftSocket, &filename_len, sizeof(uint32_t), 0) <= 0)
    return;
  if (send(receiverftSocket, &filename_len, sizeof(uint32_t), 0) < 0)
    return;
  if(recv(senderftSocket, filename, filename_len, 0) <= 0)
    return;
  if (send(receiverftSocket, filename, filename_len, 0) < 0)
    return;
  ////////////////////////////////////////////////////////////////

  //file 
  while ((n = recv(senderftSocket, buffer, FILEBUFFERSIZE, 0)) > 0)
  {
    if (send(receiverftSocket, buffer, n, 0) < 0)
      break;
  }

}


void* handleClient(void* args)
{
  messageBlock message;
  int clientSocket = *((int*)args);
  free(args);

  while(1)
  {
      int bytesReaded = recv(clientSocket, &message, sizeof(message), 0);
    if (bytesReaded <= 0)
    {
      printf("client disconnected\n");
      break;
    }

    if (strcmp(message.messageBuffer, "file") == 0)
    {
      int senderIdx = -1;
      int receiverIdx = -1;
      pthread_mutex_lock(&server.mutex);
      for (int i = 0; i < CLIENTSN; i++)
      {
        if (server.clientSockets[i] == clientSocket)
          senderIdx = i;
        else if (server.clientSockets[i] != 0)
          receiverIdx = i;
      }
      pthread_mutex_unlock(&server.mutex);

      if (senderIdx!=-1 && receiverIdx!=-1)
      {
        forwardMessage(&message, clientSocket);
        handleFileTransfer(server.ftSockets[senderIdx], server.ftSockets[receiverIdx]);
      }
    }
    else 
      forwardMessage(&message, clientSocket);

  }

  pthread_mutex_lock(&server.mutex);
  for (int i = 0; i < CLIENTSN; i++)
  {
    if (server.clientSockets[i] == clientSocket)
    {
      close(server.clientSockets[i]);
      close(server.ftSockets[i]);
      server.clientSockets[i] = 0;
      server.ftSockets[i] = 0;
      server.connectedClients--;
      break;
    }
  }

  pthread_mutex_unlock(&server.mutex);


  return NULL;
}

int main(int argc, char *argv[])
{
  //usage serverfilename <port>

  int listeningSocket, ftSocket; //listening socket and socket for connected client, ft - file transfering
  int cSockets[2];

  int port;
  //threads in order to make our clients communicate asynchronically
  pthread_t thread1, thread2;

  if (argc>1)
    port = atoi(argv[1]);
  else
    port = 8080;


  listeningSocket = serverSocketSetup(port);
  ftSocket = serverSocketSetup(port+1);

  printf("Server listening port %hu, files are shared with %hu port\n", port, port+1);
    
  //////////////////////////////////////////////////////////////
  ///              CONNECTION OF ALL SOCKETS                  //
  ///////////////////////////////////////////////////////////// 
  for(;;)
  {
    struct sockaddr_in clientAddr;
    socklen_t clientlen = sizeof(clientAddr);

    //accept main connection for messages 
    int* clientSocket = malloc(sizeof(int));
    if (clientSocket==NULL)
      error("malloc for client socket cannot me created");

    *clientSocket = accept(listeningSocket, (struct sockaddr*)&clientAddr, &clientlen);
    if (*clientSocket<0)
      error("acception failed");
  

    //accept file transfer connection
    int filetransferSocket = accept(ftSocket, (struct sockaddr*)&clientAddr, &clientlen);
    if (filetransferSocket < 0)
      error("file transfer acception failed");

    
    pthread_mutex_lock(&server.mutex);
    if (server.connectedClients >= CLIENTSN)
    {
      close(*clientSocket);
      close(filetransferSocket);
      free(clientSocket);
      pthread_mutex_unlock(&server.mutex);
      continue;
    }

    //storing all sockets with user connection to server structure
    for (int i = 0; i < CLIENTSN; i++)
    {
      if (server.clientSockets[i] ==0)
      {
        server.clientSockets[i] = *clientSocket;
        server.ftSockets[i] = filetransferSocket;
        break;
      }
    }
    server.connectedClients++;
    pthread_mutex_unlock(&server.mutex);

    pthread_t thread;
    if (pthread_create(&thread, NULL, handleClient, clientSocket) != 0)
    {
      perror("thread creation failed");
      free(clientSocket);
      continue;
    }
    pthread_detach(thread);
    printf("new client connected.");
    

  }


  pthread_mutex_destroy(&server.mutex);
  close(ftSocket);
  close(listeningSocket);
}



