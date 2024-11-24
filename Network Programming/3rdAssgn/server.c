#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 12345
#define BUFFER_SIZE 1024

int main(){
    int sockfd;
    char buffer[BUFFER_SIZE];
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len;
    
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sockfd < 0){
        perror("Socket Creation Failed!");
        exit(EXIT_FAILURE);
    }
    
    memset(&server_addr, 0, sizeof(server_addr));
    memset(&client_addr, 0, sizeof(client_addr));
    
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;
    
    if(bind(sockfd, (const struct sockaddr*)&server_addr, sizeof(server_addr)) < 0){
        perror("Bind failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    
    addr_len = sizeof(client_addr);
    
    while(1){
        int n = recvfrom(sockfd, (char*)buffer, BUFFER_SIZE, 0, (struct sockaddr*)&client_addr, &addr_len);
        buffer[n] = '\0';  // Correct null-termination
        printf("Client sent: %s\n", buffer);
        
        // Echoing back the received message
        sendto(sockfd, (const char*)buffer, strlen(buffer), 0, (const struct sockaddr*)&client_addr, addr_len);
        printf("Echoed back to client: %s\n", buffer);
    }
    
    close(sockfd);
    return 0;
}
