





typedef struct Account_{

        char *name;
        float balance;
        int isInSession;
} Account;


int open (char* accountName);
int start (char* accountName);
int creditAmount(int amount);
int debitAmount(int amount);
void balance();
void finish();
void exit();


int addAccount();
void printAccounts();

void* printStatusThread(void* arg);
void* clientListenerThread(void *arg);
void* clientSessionThread(void *arg);
void handleUserCommand(char *command, char *accOrNum, int sockfd);
