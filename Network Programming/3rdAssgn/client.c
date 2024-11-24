#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 12345
#define BUFFER_SIZE 1024

int main(){
    int sockfd;
    char *message = "HELLO, UDP SERVER!";
    struct sockaddr_in server_addr;
    socklen_t addr_len;
    
    // Create socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sockfd < 0){
        perror("Socket Creation Failed!");
        exit(EXIT_FAILURE);
    }
    
    // Clear server address struct
    memset(&server_addr, 0, sizeof(server_addr));
    
    // Set up server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;
    
    addr_len = sizeof(server_addr);
    
    // Send message to the server
    sendto(sockfd, (const char*)message, strlen(message), 0, (const struct sockaddr*)&server_addr, addr_len);
    printf("Message sent to server: %s\n", message);  // Print the correct message
    
    // Close the socket
    close(sockfd);
    return 0;
}
