#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>


#define PORT 8080
#define BUFFERSIZE 1024
#define HTMLFILE 16384
#define NAMESIZE 64


//TODO:
//  add routing
//      nahhhh its really easy, i already had done this thing that i can server different page but i specify it on start of server, so i just need to make it client-side, maybe code is fuking garbage but i understand basic principe 


void error(const char* message)
{
  perror(message);
  exit(1);
}

int getLocalPage(const char* filename, char* buffer)
{
  FILE* file;
  int n;

  file = fopen(filename, "r");
  if (file==NULL){
    perror("can't open html page");
    return -1;
  }
  if((n = fread(buffer, 1, HTMLFILE, file)) < 0){
    perror("cant read from file");
    return -1;
  }
  fclose(file);
  

  return n;
}



int handleConnection(int socket, char* filename)
{
  char buffer[BUFFERSIZE]={0};
  char method[10]={0};
  char path[100]={0};
  char version[10]={0};

  if(read(socket, buffer, BUFFERSIZE) < 0) { 
    perror("can't read from socket");
    return -1;
  
  }

  sscanf(buffer, "%s %s %s", method, path, version);
  printf("METHOD: %s\n", method);
  printf("PATH: %s\n", path);
  printf("VERSION: %s\n", version);


  /// \r\n\r\n - blank line that separate body content from headers
  if (strcmp(method, "GET") == 0) {
    char* filebuffer = (char*)malloc(HTMLFILE);

    int bytesRead = getLocalPage(filename, filebuffer);
    filebuffer[bytesRead] = '\0';
    if (bytesRead < 0)
    {
      free(filebuffer);
      return -1;
    }
    char* headers = "HTTP/1.1 200 OK\r\n"
                        "Content-Type: text/html\r\n\r\n";
    size_t responceSize = bytesRead + strlen(headers) + 1;
    char* responce = (char*)malloc(responceSize);
    if (responce == NULL)
    {
      perror("memory allocation for responce buffer failed");
      free(filebuffer);
      return -1;
    }
    strcpy(responce, headers);
    strcat(responce, filebuffer);
    
  
    if(write(socket, responce, responceSize) < 0){
      perror("cannot responce to socket");
      free(filebuffer);
      free(responce);
      return -1;
    }

    free(filebuffer);
    free(responce);
  }
  else if (strcmp(method, "POST") == 0) {
    char *body = strstr(buffer, "\r\n\r\n");
    if (body) {
        body += 4; // Move past the \r\n\r\n
        printf("Received POST data: %s\n", body);
    }
        
        // Send response
    char *response = "HTTP/1.1 200 OK\r\n"
                        "Content-Type: text/plain\r\n\r\n"
                        "POST request received";
    if(write(socket, response, strlen(response)) < 0)
      error("response to post req failed");
  }
}


int setupListeningSocket(int *lsocket)
{
  struct sockaddr_in address;
  int addrlen = sizeof(address);

  if ((*lsocket = socket(AF_INET, SOCK_STREAM, 0)) == 0){
    perror("cannot create listening socket");
    return -1;
  }
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(PORT);

  if (bind(*lsocket, (struct sockaddr*)&address, sizeof(address)) < 0){
    perror("binding failed");
    return -1;
  }
  return 1;
}


int main(int argc, char* argv[])
{
  char *filename = (char*)malloc(NAMESIZE);
  int lsocket, csocket;
  struct sockaddr_in clientAddress;
  int clientAddressLength = sizeof(clientAddress);
  
  
  printf("enter filename of html file");
  bzero(filename, NAMESIZE);
  fgets(filename, NAMESIZE, stdin);
  filename[strcspn(filename, "\n")] = 0;
  
  if (setupListeningSocket(&lsocket) == -1)
    error("error occured");

  if (listen(lsocket, 3)<0)
    error("listening failed");
  else 
    printf("listening on port %d", PORT);


  while (1)
  {
    if ((csocket = accept(lsocket, (struct sockaddr*)&clientAddress, (socklen_t*)&clientAddressLength)) < 0)
      error("acception failed");
  
    handleConnection(csocket, filename);

    close(csocket);
  }

  
    


  close(lsocket);
  return 0;
}
