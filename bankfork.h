







void printAccounts();
void clientSession();
void exitClient();



void* printStatusThread(void* arg);
void* clientListenerThread(void *arg);
void handleClientCommards(char *command, char *accOrNum, int sockfd){
void handleCustomerCommards(char *command, char *accOrNum, int sockfd){
