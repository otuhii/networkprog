#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/ip.h>
#include <netinet/if_ether.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <net/ethernet.h>
#include <unistd.h>
#include <netdb.h>


#define BUFFERSIZE 65536



void error(const char* message)
{
  perror(message);
  exit(1);
}


void handlePacket(unsigned char* buffer, int buflen)
{
  //[Ethernet Header][IP Header][TCP/UDP Header][Data/Payload]
  //Buffer in memory:
  //[00:11:22:33:44:55][66:77:88:99:AA:BB][08:00][45:00:...]...
  //|                                           |          |     
  //|<------- Ethernet Header (14 bytes) ------>|<-- IP -->|

  struct ethhdr *eth = (struct ethhdr*)buffer;//so we cast it and can access variables in this struct

  if(ntohs(eth->h_proto) == ETH_P_IP)
  {
    struct iphdr *iph = (struct iphdr*)(buffer+sizeof(struct ethhdr));
  
    //storing source and dest addr
    struct sockaddr_in source, dest;
    memset(&source, 0, sizeof(source));
    memset(&dest, 0, sizeof(dest));

    source.sin_addr.s_addr = iph->saddr;
    dest.sin_addr.s_addr = iph->daddr;

    ////////////////////////////////////      
    
    char hostBuffer[NI_MAXHOST];
    char serviceBuffer[NI_MAXSERV];
    

    printf("\nIP PACKET\n");


    if (getnameinfo((struct sockaddr*)&source, sizeof(source), hostBuffer, sizeof(hostBuffer), serviceBuffer, sizeof(serviceBuffer), NI_NUMERICHOST))
      printf("  |-Source IP       : %s:%s\n", hostBuffer, serviceBuffer);
    else 
      printf("  |-Source IP       : %s\n", inet_ntoa(source.sin_addr));



    if (getnameinfo((struct sockaddr*)&dest, sizeof(dest), hostBuffer, sizeof(hostBuffer), serviceBuffer, sizeof(serviceBuffer), NI_NUMERICHOST))
      printf("   |-Destination IP  : %s:%s\n", hostBuffer, serviceBuffer);
    else
      printf("  |-Destination IP  : %s\n", inet_ntoa(dest.sin_addr));


    printf("  |-Protocol        : %d\n", iph->protocol);
    printf("  |-TTL             : %d\n", iph->ttl);
    printf("  |-Total length    : %d\n", htons(iph->tot_len));

  }

}

int main(int argc, char* argv[])
{
  int sock;
  struct sockaddr addr;
  int saddr_len = sizeof(addr);

  unsigned char* buffer = (unsigned char*)malloc(BUFFERSIZE); 

  sock = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
  if (sock < 0)
    error("cant open socket");

  printf("Socket started listening");

  for(;;)
  {
    int buflen = recvfrom(sock, buffer, BUFFERSIZE, 0, &addr, (socklen_t*)&saddr_len);
    if (buflen < 0)
      error("couldnt receive from socket");

    handlePacket(buffer, buflen);
  }

  close(sock);
  free(buffer);
  return 0;
}


