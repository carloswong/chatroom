#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[]) {
	
	int sockfd,client_sock, n, c;
	struct sockaddr_in server_addr, client;
	char *message;

	sockfd = socket(AF_INET,SOCK_STREAM,0);
	if(!sockfd) {
		puts("Create Sock Failed");
		return 0;
	}

	int optval = 1;
	setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR, &optval, sizeof(optval));

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(8080);
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	if(bind(sockfd,(struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
		puts("Bind failed");
		return 0;
	}

	listen(sockfd,3);

	c = sizeof(struct sockaddr_in);
	puts("Watting for connecting");
	while ( (client_sock = accept(sockfd, (struct sockaddr *)&client, (socklen_t *)&c)) ) {
		puts("Connection Accepted");
		message = "Response from server\n";
		int errorcode = write(client_sock, message, strlen(message));
		if(errorcode < 0) {
			printf("errorcode %d\n",errorcode);
		}

		close(client_sock);
	}

	return 0;
}
