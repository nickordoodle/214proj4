





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

typedef struct Bank_{

        Account *head;
        int numAccounts;
} Bank;

int addAccount();
void printAccounts();
