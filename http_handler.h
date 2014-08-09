#define DOCROOT  "./http"

typedef struct {
	char *status;
	char *status_msg;
	char *protocol;
} http_response_header;

typedef struct {
	http_response_header header;
	char *server;
	char *charset;
	char *content_type;
	char *connection;
	char *body;
} http_respone_message;

http_respone_message get_response_message();
void not_found(int sock, char *filename);
void head(char *path, char *message);
void get(int sock, char *path, char *message);
void post(char *path, char *message);