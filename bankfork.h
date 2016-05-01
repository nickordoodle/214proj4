







void printAccounts();
void clientSession();
void exitClient();

void open(char * clientMsg, char* acc);
void start(char * clientMsg, char* acc);
void credit(char * clientMsg, char* num);
void debit(char * clientMsg, char* num);
void balance(char * clientMsg);
void finish(char * client Msg);
void exitClient(char * clientMsg);


void* printStatusThread(void* arg);
void* clientListenerThread(void *arg);
void handleuserCommands(char *command, char *accOrNum, int sockfd);


