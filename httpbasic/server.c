#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>


#define PORT 8080
#define BUFFERSIZE 1024


void error(const char* message)
{
  perror(message);
  exit(1);
}


int handleConnection(int socket)
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
  printf("METHOD: %s", method);
  printf("PATH: %s", path);
  printf("VERSION: %s", version);
  

  /// \r\n\r\n - blank line that separate body content from headers
  if (strcmp(method, "GET") == 0) {
    char *response = "HTTP/1.1 200 OK\r\n"
                        "Content-Type: text/html\r\n\r\n"
                        "<html><body><h1>HELLO WORLD!</h1></body></html>";
    if(write(socket, response, strlen(response)) < 0){
      perror("cannot responce to socket");
      return -1;
    }
  }
  else if (strcmp(method, "POSR") == 0) {
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
  char *hostname;
  int lsocket, csocket;
  struct sockaddr_in clientAddress;
  int clientAddressLength = sizeof(clientAddress);
  

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
  
    handleConnection(csocket);

    close(csocket);
  }

  
    


  close(lsocket);
  return 0;
}
