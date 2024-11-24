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
    struct sockaddr_in broadcastaddr;
    char buff[N] = {0};

    // Create the socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        err_log("Socket creation failed");
    }

    // Set up the broadcast address
    memset(&broadcastaddr, 0, sizeof(broadcastaddr));
    broadcastaddr.sin_family = AF_INET;
    if (inet_pton(AF_INET, "192.168.1.255", &broadcastaddr.sin_addr) <= 0) {
        err_log("Invalid broadcast address");
    }
    broadcastaddr.sin_port = htons(PORT);

    // Enable broadcast option on the socket
    int optval = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &optval, sizeof(int)) < 0) {
        err_log("Error setting socket options for broadcast");
    }

    while (1) {
        // Read input from the user
        printf("input: ");
        fgets(buff, N, stdin);

        // Remove the newline character from the input buffer (if present)
        buff[strcspn(buff, "\n")] = 0;

        // Send the message to the broadcast address
        if (sendto(sockfd, buff, strlen(buff), 0, (struct sockaddr*)&broadcastaddr, sizeof(broadcastaddr)) < 0) {
            err_log("Error sending the message");
        }
    }

    // Close the socket
    close(sockfd);
    return 0;
}
