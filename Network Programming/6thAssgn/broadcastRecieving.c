#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 12345
#define N 1024
#define err_log(log) do{perror(log); exit(1);} while(0)

int main(int argc, const char *argv[]) {
    int sockfd;
    struct sockaddr_in broadcastaddr, srcaddr;
    char buff[N] = {0};
    int optval = 1;
    
    // Create the socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        err_log("Socket creation failed");
    }
    
    // Enable broadcast on the socket
    if (setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &optval, sizeof(optval)) < 0) {
        err_log("Error setting socket options for broadcast");
    }
    
    // Set up the broadcast address
    memset(&broadcastaddr, 0, sizeof(broadcastaddr));
    broadcastaddr.sin_family = AF_INET;
    broadcastaddr.sin_addr.s_addr = INADDR_ANY;  // Listen on all interfaces
    broadcastaddr.sin_port = htons(PORT);
    
    // Bind the socket
    if (bind(sockfd, (struct sockaddr*)&broadcastaddr, sizeof(broadcastaddr)) < 0) {
        err_log("Error binding the socket");
    }
    
    socklen_t addrlen = sizeof(srcaddr);
    
    // Listen for incoming messages in a loop
    while (1) {
        if (recvfrom(sockfd, buff, N, 0, (struct sockaddr*)&srcaddr, &addrlen) < 0) {
            err_log("Error receiving the message");
        }
        printf("Received message: %s from %s:%d\n", buff, inet_ntoa(srcaddr.sin_addr), ntohs(srcaddr.sin_port));
    }

    close(sockfd);
    return 0;
}
