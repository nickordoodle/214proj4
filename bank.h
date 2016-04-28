





typedef struct Account_{

        char *name;
        float balance;
        int isInSession;
} Account;

typedef struct Bank_{

        Account *head;
        int numAccounts;
} Bank;


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
void handleUserCommand(char *command);
