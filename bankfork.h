







void printAccounts();
void clientSession();
void exitClient();

void openfnc(char * clientMsg, char* acc);
void startfnc(char * clientMsg, char* acc);
void credit(char * clientMsg, char* num);
void debit(char * clientMsg, char* num);
void balance(char * clientMsg);
void finish(char * client Msg);
void exitClient(char * clientMsg);


void* printStatusThread(void* arg);
void* clientListenerThread(void *arg);
void handleUserCommands(char *command, char *accOrNum, int sockfd);


