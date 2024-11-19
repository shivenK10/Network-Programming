#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>

#define PORT 80

int main(int argc, char const *argv[]) {
    int server_fd, new_socket;
    long valread;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    
    // Shortened HTTP response
    char *server_response = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: 38\n\n<!DOCTYPE HTML><html><body><h1>Hello World</h1></body></html>";
    
    char buffer[30000] = {0};
    
    // Step 1: Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("In socket");
        exit(EXIT_FAILURE);
    }
    
    // Step 2: Define the address
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    
    // Step 3: Bind the socket to the address
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("In bind");
        exit(EXIT_FAILURE);
    }
    
    // Step 4: Listen for incoming connections
    if (listen(server_fd, 10) < 0) {
        perror("In listen");
        exit(EXIT_FAILURE);
    }
    
    printf("HTTP Server is listening on port %d...\n", PORT);
    
    // Infinite loop to accept and process client requests
    while (1) {
        printf("Waiting for a connection...\n");
        
        // Step 5: Accept client request
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            perror("In accept");
            exit(EXIT_FAILURE);
        }
        
        // Step 6: Read data from the client (HTTP request)
        valread = read(new_socket, buffer, 30000);
        printf("Received Request:\n%s\n", buffer);
        
        // Step 7: Send HTTP response
        write(new_socket, server_response, strlen(server_response));
        printf("-------Response sent to client---------\n");
        
        // Step 8: Close the socket
        close(new_socket);
    }
    
    return 0;
}
