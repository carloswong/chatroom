#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include "service.h"
#include "parser.h"
#include "http_handler.h"

int setup_service(void) {
	int sockfd,client_socket, n, c, *client_sock;
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
	server_addr.sin_port = htons(SERVICE_PORT);
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	if(bind(sockfd,(struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
		puts("Bind failed");
		return 0;
	}

	listen(sockfd,3);

	printf("Chatroom Service run at:%d\n",SERVICE_PORT);
	c = sizeof(struct sockaddr_in);
	while ( (client_socket = accept(sockfd, (struct sockaddr *)&client, (socklen_t *)&c)) ) {
		puts("connection accepted");

		pthread_t sniffer_thread;
		// client_sock = malloc(1);
		*client_sock = client_socket;

		if(pthread_create(&sniffer_thread, NULL, connection_handler, (void *)client_sock) < 0) {
			perror("could not create thread");
			return 1;
		}
		//close(client_sock);
	}

	if (client_socket < 0) {
		perror("accept failed");
		return 0;
	}

	return 1;
}

void *connection_handler(void *client_sock) {
	int sock = *(int *)client_sock;
	int read_size;
	char client_message[CLIENT_MESSAGE_MAX_LEN];

	if ((read_size = recv(sock, client_message, CLIENT_MESSAGE_MAX_LEN, 0)) > 0) {
		
		char http_message[20][100];
		char header[4][100];
		char *message = malloc(1024);

		parse_message(client_message, http_message);
		parse_header(http_message[0], header);

		if(strcmp(header[0],"GET") == 0) {
			get(sock, header[1], message);
		}else {
			not_found(sock,"");
		}
		free(message);
	}

	if(read_size == 0) {
		puts("client disconnected");
		fflush(stdout);
	}else if(read_size == -1) {
		puts("recv failed");
	}

	close(sock);
	return 0;
}

