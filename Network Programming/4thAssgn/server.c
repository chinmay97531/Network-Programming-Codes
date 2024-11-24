#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 1234
#define BUFFER_SIZE 1024

int main(){
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};
    
    // Creating socket
    if((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0){
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }
    
    // Set up server address
    address.sin_family = AF_INET;
    address.sin_port = htons(PORT);
    address.sin_addr.s_addr = INADDR_ANY;
    
    // Binding socket
    if(bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0){
        perror("Bind Failed");
        exit(EXIT_FAILURE);
    }
    
    // Listening for connections
    if(listen(server_fd, 3) < 0){
        perror("Listen Failed");
        exit(EXIT_FAILURE);
    }
    
    printf("Server is listening on port %d.........\n", PORT);
    
    // Accept client connection
    if((new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen)) < 0){
        perror("Accept Failed");
        exit(EXIT_FAILURE);
    }
    
    printf("Client Connected\n");
    
    // Communication loop
    while(1){
        memset(buffer, 0, BUFFER_SIZE);
        
        // Reading data from client
        int valread = read(new_socket, buffer, BUFFER_SIZE);
        if(valread <= 0) {
            perror("Read failed or connection closed");
            break;
        }

        // If client sends "exit", close the connection
        if(strcmp(buffer, "exit\n") == 0){
            printf("Client has exited the chat\n");
            break;
        }
        
        // Display client message
        printf("Client: %s", buffer);
        
        // Get server's response
        printf("Server: ");
        fgets(buffer, BUFFER_SIZE, stdin);
        
        // Send response to client
        send(new_socket, buffer, strlen(buffer), 0);
        
        // If server sends "exit", close the connection
        if(strcmp(buffer, "exit\n") == 0){
            printf("Exiting Chat....\n");
            break;
        }
    }
    
    // Close the sockets
    close(new_socket);
    close(server_fd);
    return 0;
}
