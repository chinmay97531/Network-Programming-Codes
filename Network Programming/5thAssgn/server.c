#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

#define PORT 12345
#define BUFFER_SIZE 1024

int main(){
	int sockfd;
	char buffer[BUFFER_SIZE];
	struct sockaddr_in servaddr, cliaddr;
	
	if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
		perror("Socket creation failed");
		exit(EXIT_FAILURE);
	}
	
	memset(&servaddr, 0, sizeof(servaddr));
	memset(&cliaddr, 0, sizeof(cliaddr));
	
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(PORT);
	servaddr.sin_addr.s_addr = INADDR_ANY;
	
	if(bind(sockfd, (const struct sockaddr*)&servaddr, sizeof(servaddr)) < 0){
		perror("Bind Failed");
		close(sockfd);
		exit(EXIT_FAILURE);
	}
	
	int n;
	socklen_t len = sizeof(cliaddr);
	
	while(1){
		memset(buffer, 0, sizeof(buffer));
		
		n = recvfrom(sockfd, (char*)buffer, BUFFER_SIZE, MSG_WAITALL, (struct sockaddr*) &cliaddr, &len);
		buffer[n] = '\0';
		printf("Client: %s", buffer);
		
		if(strcmp(buffer, "exit") == 0){
			printf("Connection closed by client.....");
			break;
		}
		
		printf("Server: ");
		fgets(buffer, BUFFER_SIZE, stdin);
		
		sendto(sockfd, (char*)buffer, strlen(buffer), 0, (const struct sockaddr*)&cliaddr, len);
		
		if(strcmp(buffer, "exit") == 0){
			printf("Connection closed.....");
			break;
		}
	}
	
	close(sockfd);
	return 0;
}
