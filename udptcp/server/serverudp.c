#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define IP_PROTOCOL 0
#define PORT_NO 15050
#define NET_BUF_SIZE 1500
#define sendrecvflag 0
#define IP_ADDRESS "127.0.0.1" // localhost
#define nofile "File Not Found!"

void clearBuf(char* b)
{
  int i;
  for (i = 0; i < NET_BUF_SIZE; i++)
    b[i] = '\0';
}

int sendFile(FILE* fp, char* buf, int s)
{
  int i, len;
  if (fp == NULL) {
    strcpy(buf, nofile);
    len = strlen(nofile);
    buf[len] = EOF;
    for (i = 0; i <= len; i++)
      buf[i] = buf[i];
    return 1;
  }

  char ch;
  for (i = 0; i < s; i++) {
    ch = fgetc(fp);
    buf[i] = ch;
    if (ch == EOF)
      return 1;
  }
  return 0;
}

int main()
{
  int sockfd, nBytes;
  struct sockaddr_in addr_con;
  int addrlen = sizeof(addr_con);
  addr_con.sin_family = AF_INET;
  addr_con.sin_port = htons(PORT_NO);
  addr_con.sin_addr.s_addr = inet_addr(IP_ADDRESS);
  char net_buf[NET_BUF_SIZE];
  FILE* fp;

  sockfd = socket(AF_INET, SOCK_DGRAM, IP_PROTOCOL);

  while (1) {
    printf("\nDigite o nome do arquivo: \n");

    clearBuf(net_buf);
    scanf("%s",net_buf);
    printf("%s \n",net_buf);

    fp = fopen(net_buf, "r");

    if (fp == NULL)
      printf("\nArquivo não exite!\n");
    else
      printf("\nPreparando envio!\n");

    while(1){

      if(sendFile(fp, net_buf, NET_BUF_SIZE)){
          sendto(sockfd, net_buf, NET_BUF_SIZE, sendrecvflag, (struct sockaddr*)&addr_con, addrlen);
          break;
      }

      sendto(sockfd, net_buf, NET_BUF_SIZE, sendrecvflag, (struct sockaddr*)&addr_con, addrlen);
      clearBuf(net_buf);
    }
    if(fp != NULL){
      fclose(fp);
    }
  }
  return 0;
}
