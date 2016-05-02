#include "sorted-list.h"


/*creates newClient to be added to the BST*/
Client* createClient(char *clientName, int clientCount){
        Client *newClient = malloc(sizeof(Client));
        newClient->left = NULL;
        newClient->right = NULL;

        newClient ->name = malloc(sizeof(char) * strlen(clientName));
        strcpy(newClient -> name, clientName);
        newClient->balance = 0;
        newClient->inuse = 0;
        newClient->index = clientCount;

        return newClient;
}

/*returns 0 if successful 2 if client name already exsisted*/
int open(Client *curr,int clientCount, char* clientName){
    
    if(clientCount == 0){
        curr = createClient(clientName, clientCount);
        return 0;
    }

    int compareVal = strcmp(curr->name, clientName);

    /* Insert into left subtree */
    if(compareVal < 0){
        if(curr -> left  == NULL){
                curr -> left = createClient(clientName,clientCount);
                return 0;
        } else{
            return open(curr->left, clientCount, clientName);
 }
    }

    /* Insert into right subtree */
    else if (compareVal > 0){
        if(curr -> right == NULL){
                curr -> right = createClient(clientName, clientCount);
                return 0;
        } else{
           return open(curr->right, clientCount, clientName);
        }

    }
    /* Client has already previously occurred */
   return 2;
}



/*returns the Client struct if found returns NULL if not found*/
Client* start(Client *curr, int clientCount, char* clientName){
    int compareVal = strcmp(curr->name, clientName);

    /* Insert into left subtree */
    if(compareVal < 0){
        if(curr -> left  == NULL){
                return NULL;
        } else{
            return start(curr->left, clientCount, clientName);
        }
    }

    /* Insert into right subtree */
    else if (compareVal > 0){
        if(curr -> right == NULL){
                return NULL;
        } else{
           return start(curr->right, clientCount, clientName);
        }

    }
    /* Client has previously been opened */
    if(curr->inuse == 1)
        return NULL;
    curr->inuse = 1;
    return curr;
}





void print(Client * head){
        if(head == NULL){
                printf("No client accounts are opened in the bank at this time.\n");
                return;
        }
        printRecurs(head);

        return;
}

void printRecurs(Client* start){
        Client * curr = start;

        if(curr->left != NULL)
                return printRecurs(curr->left);
        if(curr->inuse == 0)
                printf("Client:%s: \n\tBalance:%f\n\tIn use: no\n",curr->name,curr->balance);
        else
                printf("Client:%s: \n\tBalance:%f\n\tIn use: yes\n",curr->name,curr->balance);

        if(curr->right != NULL)
                return printRecurs(curr->right);
}
