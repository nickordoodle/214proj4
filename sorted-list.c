#include "sorted-list.h"

Client *head = NULL;
int clientCount = 0;

/*creates newClient to be added to the BST*/
Client* createClient(char *clientName){
        Client *newClient = malloc(sizeof(Client));
        newClient->left = NULL;
        newClient->right = NULL;

        newClient ->name = malloc(sizeof(char) * strlen(clientName));
        strcpy(newClient -> name, clientName);
        newClient->balance = 0;
        newClient->inuse = 0;
        newClient->index = clientCount;
        clientCount++;

        return newClient;
}

/*returns 0 if successful 1 if already reached max clients 2 if account with that name already exsits*/
int open(char *clientName){

      if(clientCount == 20){
              return 1;
      }


       return insertClient(head, clientName);
}
/*returns 0 if successful 2 if client name already exsisted*/
int insertClient(Client *curr, char* clientName){
    
    if(!head){
        head = createClient(clientName);
        return 0;
    }

    int compareVal = strcmp(curr->name, clientName);

    /* Insert into left subtree */
    if(compareVal < 0){
        if(curr -> left  == NULL){
                curr -> left = createClient(clientName);
                return 0;
        } else{
            return insertClient(curr->left, clientName);
 }
    }

    /* Insert into right subtree */
    else if (compareVal > 0){
        if(curr -> right == NULL){
                curr -> right = createClient(clientName);
                return 0;
        } else{
           return insertClient(curr->right, clientName);
        }

    }
    /* Client has already previously occurred */
   return 2;
}


Client* start(char* clientName){
    return accessClient(head,clientName);       
}


/*returns the Client struct if found returns NULL if not found*/
Client* accessClient(Client *curr, char* clientName){
    int compareVal = strcmp(curr->name, clientName);

    /* Insert into left subtree */
    if(compareVal < 0){
        if(curr -> left  == NULL){
                return NULL;
        } else{
            return accessClient(curr->left, clientName);
        }
    }

    /* Insert into right subtree */
    else if (compareVal > 0){
        if(curr -> right == NULL){
                return NULL;
        } else{
           return accessClient(curr->right, clientName);
        }

    }
    /* Client has previously been opened */
    if(curr->inuse == 1)
        return NULL;
    curr->inuse = 1;
    return curr;
}





void print(){
        if(clientCount == 0){
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
