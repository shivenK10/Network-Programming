#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 80
#define SERVER_ADDRESS "127.0.0.1"

int main() {
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char *request = "GET / HTTP/1.1\r\nHost: localhost\r\n\r\n";
    char buffer[1024] = {0};

    // Step 1: Create socket file descriptor
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }

    // Step 2: Define server address
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    
    // Convert IPv4 address from text to binary form
    if (inet_pton(AF_INET, SERVER_ADDRESS, &serv_addr.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    // Step 3: Connect to the server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }

    // Step 4: Send the HTTP GET request
    send(sock, request, strlen(request), 0);
    printf("HTTP request sent\n");

    // Step 5: Read the server's response
    valread = read(sock, buffer, 1024);
    printf("Server response:\n%s\n", buffer);

    // Step 6: Close the socket
    close(sock);

    return 0;
}
