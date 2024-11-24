#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

#define PORT 1234
#define BUFFER_SIZE 1024

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0};

    // Create socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("Socket Creation Failed\n");
        return -1;
    }

    // Setup server address
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IP address (localhost for local testing)
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {  // Use 127.0.0.1 for localhost
        printf("Invalid Address\n");
        return -1;
    }

    // Connect to server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("Connection failed\n");
        return -1;
    }

    printf("Connected to server....\n");

    // Communication loop
    while (1) {
        printf("Client: ");
        fgets(buffer, BUFFER_SIZE, stdin);
        send(sock, buffer, strlen(buffer), 0);

        // Exit condition
        if (strcmp(buffer, "exit\n") == 0) {
            printf("Connection Closed....\n");
            break;
        }

        memset(buffer, 0, BUFFER_SIZE);

        // Read server response
        int valread = read(sock, buffer, BUFFER_SIZE);
        if (valread <= 0) {
            printf("Error reading from server\n");
            break;
        }

        if (strcmp(buffer, "exit\n") == 0) {
            printf("Connection Closed......\n");
            break;
        }

        printf("Server: %s", buffer);
    }

    // Close the socket
    close(sock);
    return 0;
}
