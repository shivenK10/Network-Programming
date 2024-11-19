#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>

int main()
{
    char buff[80];
    int sockfd, n;
    socklen_t len; 
    struct sockaddr_in servaddr;
    const char *server_ip = "127.0.0.1"; 

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(server_ip);
    servaddr.sin_port = htons(43454);
    len = sizeof(servaddr);

    printf("\nEnter string: ");
    fgets(buff, sizeof(buff), stdin);

    sendto(sockfd, buff, sizeof(buff), 0, (struct sockaddr *)&servaddr, len);
    bzero(buff, sizeof(buff));
    recvfrom(sockfd, buff, sizeof(buff), 0, (struct sockaddr *)&servaddr, &len);
    printf("From Server: %s\n", buff);

    close(sockfd);
}
