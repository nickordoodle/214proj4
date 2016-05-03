void *userCommandThread(void *input);
void *serverResponseThread(void *input);
int isValidCommand(char *command);
void end();
void error(char *msg);
