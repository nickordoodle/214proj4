





typedef struct Account_{

        char *name;
        float balance;
        int isInSession;
} Account;


void* printStatusThread(void* arg);
void* clientListenerThread(void *arg);
void* clientSessionThread(void *arg);
void handleUserCommand(char *command, char *accOrNum, int sockfd);
