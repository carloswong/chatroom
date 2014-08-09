#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"

void parse_message(char *message, char http_message[20][100]) { 
	char *token="\r\n";
	parse_raw_message(message, token, http_message);
}

void parse_header(char *header, char head[4][100]) 
{
	char *token=" ";
	parse_raw_message(header, token, head);
}

void parse_raw_message(char *haystack, char *token, char (*result)[100]) 
{
	char *tok;
	int i = 0;

	tok = strtok(haystack, token);
	while(tok) {
		int len = strlen(tok), j;
		for(j=0; j < len; j++) {
			result[i][j] = tok[j];
		}
		tok = strtok(NULL,token);
		i++;
	}
}
