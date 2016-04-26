void *userCommandThread(void *input);
void *serverResponseThread(void *input);
int isValidCommand(char *command);
void sigint_handler(int sig);
void error(char *msg);