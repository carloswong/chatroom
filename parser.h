
typedef struct {
	char *method;
	char *potocol;
	char *path;
} HttpHeader;

void parse_message(char *message, char http_message[20][100]); 

void parse_header(char *header, char head[4][100]);

void parse_raw_message(char *haystack, char *token, char (*result)[100]); 
