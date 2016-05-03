typedef struct Map__{
        pthread_mutex_t clientMutexes[20];
        pthread_mutex_t newAccountMutex;
        
        int open;
        
        int accountCount;

        char * name [20];
        float balance[20];
        int inuse[20];
}Map;

typedef struct ProcessLL_{
        int sockfd;
        pid_t child;
        processLL_ next;
}ProcessLL;


void printAccounts();
void clientSession(int arg);
void exitClient();

void print();
void open(char * clientMsg, char* acc);
void start(int sockfd, char * clientMsg, char* acc);
void credit(char * clientMsg, char* num);
void debit(char * clientMsg, char* num);
void balance(char * clientMsg);
void finish(char * clientMsg);
void exitClient(char * clientMsg);

void signalHandler();
void* printStatusThread(void* arg);
void* clientListenerThread(void *arg);
void handleUserCommands(char *command, char *accOrNum, int sockfd);
