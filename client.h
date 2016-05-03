void *userCommandThread(void *input);
void *serverResponseThread(void *input);
int isValidCommand(char *command);
void close();
void error(char *msg);
