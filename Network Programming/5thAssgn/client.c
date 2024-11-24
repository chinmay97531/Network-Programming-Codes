#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

#define PORT 12345
#define BUFFER_SIZE 1024

int main(){
    int sockfd;
    char buffer[BUFFER_SIZE];
    struct sockaddr_in servaddr;

    // Create socket
    if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = INADDR_ANY;

    int n;
    socklen_t len;

    while(1){
        memset(buffer, 0, sizeof(buffer));

        // Get user input
        printf("Client: ");
        fgets(buffer, BUFFER_SIZE, stdin);

        // Send message to server
        sendto(sockfd, buffer, strlen(buffer), 0, (const struct sockaddr*)&servaddr, sizeof(servaddr));

        // If user types "exit", close the connection
        if(strcmp(buffer, "exit") == 0){
            printf("Connection Closed......\n");
            break;
        }

        // Receive message from server
        n = recvfrom(sockfd, (char*)buffer, BUFFER_SIZE, MSG_WAITALL, (struct sockaddr*)&servaddr, &len);

        // Null-terminate the received string
        buffer[n] = '\0';
        printf("Server: %s", buffer);

        // If server sends "exit", close the connection
        if(strcmp(buffer, "exit") == 0){
            printf("Connection Closed by server......\n");
            break;
        }
    }

    // Close the socket
    close(sockfd);
    return 0;

}
