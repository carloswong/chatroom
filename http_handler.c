#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "http_handler.h"

http_respone_message get_response_message() {
	http_respone_message response;

	response.header.protocol = "HTTP/1.1";
	response.content_type = "text/html";
	response.charset = "utf-8";
	response.server = "Chatroom Server";
	response.connection = "keep-alive";
	response.body = NULL;

	return response;
}

void output_response(int sock, http_respone_message response) {
	char header[200];
	strcpy(header, response.header.protocol);
	strcat(header, " ");
	strcat(header, response.header.status);
	strcat(header, " ");
	strcat(header, response.header.status_msg);
	strcat(header, "\r\n");
	strcat(header, "Server: ");
	strcat(header, response.server);
	strcat(header, "\r\n");
	strcat(header, "Content-Type: ");
	strcat(header, response.content_type);
	strcat(header, ";charset=");
	strcat(header, response.charset);
	strcat(header, "\r\n");
	strcat(header, "Connection: ");
	strcat(header, response.connection);
	strcat(header, "\r\n\r\n");

	write(sock, header, strlen(header));

	if(response.body != NULL) {
		write(sock, response.body, strlen(response.body));
		
		char *ending = "\r\n";
		write(sock, ending, strlen(ending));
	}
}

void not_found(int sock, char *filename) {
	http_respone_message response = get_response_message();
	response.header.status = "404";
	response.header.status_msg = "Not Found";
	response.body = "<html><head><title>404 Not Found</title></head><body><h1>404 Not Found</h1></body></html>";

	output_response(sock, response);
}

void internal_error(int sock) {
	http_respone_message response = get_response_message();
	response.header.status = "500";
	response.header.status_msg = "Internal Error";
	response.body = "<html><head><title>500 Internal Error</title></head><body><h1>500 Internal Error</h1></body></html>";

	output_response(sock, response);
}

void get(int sock, char *path, char *message) {
	if(strcmp(path, "/") == 0) {
		strcpy(path,"/index.html");
	}
	char filename[100];
	
	strcpy(filename, DOCROOT);
	strcat(filename,path);

	FILE *stream = fopen(filename,"rb+");
	if(stream == NULL) {
		not_found(sock, filename);
		return;
	}

	http_respone_message response = get_response_message();
	response.header.status = "200";
	response.header.status_msg = "OK";
	output_response(sock, response);

	while(!feof(stream)) {
		char line[4096];
		if(fgets(line, 4096, stream) != NULL) {
			write(sock, line, strlen(line));
		}
	}
	fclose(stream);

	char *ending = "\r\n";
	write(sock, ending, strlen(ending));
}


void head(char *path, char *message) {
}


void post(char *path, char *message) {
}
