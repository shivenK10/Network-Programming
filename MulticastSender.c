#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#define MCAST_GRP "224.0.0.1"  // Multicast group address
#define MCAST_PORT 5004        // Multicast group port

int main() {
    struct sockaddr_in mcast_addr;
    int sock;
    char *message = "Hello, Multicast!";
    int ttl = 1;  // Time-to-live

    // Create socket
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    // Set socket options for multicast
    if (setsockopt(sock, IPPROTO_IP, IP_MULTICAST_TTL, &ttl, sizeof(ttl)) < 0) {
        perror("Setting socket option failed");
        close(sock);
        exit(1);
    }

    // Configure multicast address
    memset(&mcast_addr, 0, sizeof(mcast_addr));
    mcast_addr.sin_family = AF_INET;
    mcast_addr.sin_addr.s_addr = inet_addr(MCAST_GRP);
    mcast_addr.sin_port = htons(MCAST_PORT);

    // Send multicast message
    if (sendto(sock, message, strlen(message), 0, (struct sockaddr *)&mcast_addr, sizeof(mcast_addr)) < 0) {
        perror("Message send failed");
        close(sock);
        exit(1);
    }

    printf("Message sent to multicast group: %s\n", message);

    close(sock);
    return 0;
}