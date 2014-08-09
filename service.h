#define SERVICE_PORT 8080
#define CLIENT_MESSAGE_MAX_LEN 2000

int setup_service(void);
void *connection_handler(void *);