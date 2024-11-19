#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#define MCAST_GRP "224.0.0.1"  // Multicast group address
#define MCAST_PORT 5004        // Multicast group port

int main() {
    struct sockaddr_in local_addr, mcast_group;
    struct ip_mreq mreq;
    int sock;
    char buffer[1024];
    int addr_len, recv_len;

    // Create socket
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    // Allow multiple sockets to use the same port
    int reuse = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0) {
        perror("Setting socket option failed");
        close(sock);
        exit(1);
    }

    // Configure local address structure
    memset(&local_addr, 0, sizeof(local_addr));
    local_addr.sin_family = AF_INET;
    local_addr.sin_addr.s_addr = htonl(INADDR_ANY);  // Listen on any interface
    local_addr.sin_port = htons(MCAST_PORT);

    // Bind to the multicast port
    if (bind(sock, (struct sockaddr *)&local_addr, sizeof(local_addr)) < 0) {
        perror("Bind failed");
        close(sock);
        exit(1);
    }

    // Join the multicast group
    mreq.imr_multiaddr.s_addr = inet_addr(MCAST_GRP);  // Multicast group
    mreq.imr_interface.s_addr = htonl(INADDR_ANY);     // Local interface
    if (setsockopt(sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)) < 0) {
        perror("Adding membership failed");
        close(sock);
        exit(1);
    }

    // Receive messages
    printf("Waiting for multicast messages...\n");
    while (1) {
        addr_len = sizeof(mcast_group);
        recv_len = recvfrom(sock, buffer, sizeof(buffer) - 1, 0, (struct sockaddr *)&mcast_group, &addr_len);
        if (recv_len < 0) {
            perror("Receiving message failed");
            break;
        }
        buffer[recv_len] = '\0';  // Null-terminate the received string
        printf("Received message: %s\n", buffer);
    }

    close(sock);
    return 0;
}