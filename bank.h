typedef struct Map__{
        pthread_mutex_t clientMutexes[20];
        pthread_mutex_t newAccountMutex;
        pid_t processes[20];

        
        int accountCount;

        char * name [20];
        float balance[20];
        int inuse[20];
}Map;




void printAccounts();
void clientSession();
void exitClient();

void print();
void openfnc(char * clientMsg, char* acc);
void startfnc(char * clientMsg, char* acc);
void credit(char * clientMsg, char* num);
void debit(char * clientMsg, char* num);
void balance(char * clientMsg);
void finish(char * clientMsg);
void exitClient(char * clientMsg);

void signalHandler();
void* printStatusThread(void* arg);
void* clientListenerThread(void *arg);
void handleUserCommands(char *command, char *accOrNum, int sockfd);
