#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8080
#define IP_ADDRESS "127.0.0.1" // localhost

int main(int argc, char const *argv[])
{
    int sock = 0;
    int valread;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = inet_addr(IP_ADDRESS);

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Erro na criação do socket \n");
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\n Falha na conexão \n");
        return -1;
    }

    while(1)
    {
        printf("\nEsperando arquivo:\n");
        valread = read(sock, buffer, 1024);
        printf("%s\n",buffer);
        printf("\nArquivo recebido\n");
    }
    
    return 0;
}
