#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define IP_PROTOCOL 0
#define PORT 8080
#define IP_ADDRESS "127.0.0.1" // localhost
#define NET_BUF_SIZE 1500
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

int main(int argc, char const *argv[])
{
	int sock = 0;
	int new_socket;
    int valread;
    struct sockaddr_in serv_addr;
    char net_buf[NET_BUF_SIZE];
    int addrlen = sizeof(serv_addr);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = inet_addr(IP_ADDRESS);
	FILE* fp;

	if ((sock = socket(AF_INET, SOCK_STREAM, IP_PROTOCOL)) == 0)
	{
		printf("\nCriação de socket falhou\n");
		return -1;
	}
	if (bind(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr))<0)
	{
		printf("\nBind do socket falhou\n");
		return -1;
	}
	if (listen(sock, 3) < 0)
	{
		printf("\nMuitas conexões\n");
		return -1;
	}
	if ((new_socket = accept(sock, (struct sockaddr *)&serv_addr, (socklen_t*)&addrlen))<0)
	{
		printf("\nFalha ao aceitar conexão\n");
		exit(EXIT_FAILURE);
	}
	printf("\nAguardando cliente\n");
	while(1){
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
		      send(new_socket , net_buf, strlen(net_buf), 0);
		      break;
		  }

		  send(new_socket , net_buf, strlen(net_buf), 0);
		  clearBuf(net_buf);
		}
		if(fp != NULL){
		  fclose(fp);
		}
	}
	
	return 0;
}
