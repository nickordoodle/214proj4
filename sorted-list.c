#include "sorted-list.h"

Client *head = NULL;
int clientCount = 0;

/*creates newClient to be added to the BST*/
Client createClient(clientName){
        Client *newClient = malloc(sizeof(Client));
        newClient->left = NULL; 
        newClient->right = NULL; 
        
        newClient ->name = malloc(sizeof(char) * strlen(clientName));
        strcpy(Client -> name, clientName);
        newClient->balance = 0;
      
        
        return NewClient;
}

/*returns 1 if successful 0 if not*/
int addClient(char *clientName){
      Client *newClient = createClient(clientName);
      Client *curr = NULL;
      
      if(clientCount == 20){
              return 0;        
      }
      
      clientCount++;
      
       return insertclient(head, clientName);
}
//CHANGE TO ACCESS CLIENT and haVE ADDLCLIENT CALL
/*returns 1 if successful 0 if client name already exsisted*/
int insertClient(Client *curr, char* clientName){
        int compareVal = strcmp(curr->name, clientName);

    /* Insert into left subtree */
    if(compareVal < 0){
        if(curr -> left  == NULL){
                node -> left = createClient(clientName);
                return 1;
        } else{
            return insertClient(curr->left, clientName);
        }
    }
    
    /* Insert into right subtree */
    else if (compareVal > 0){
        if(curr -> right == NULL){
                curr -> right = createClient(clientName);
                return 1;
        } else{
           returnVal = insertClient(curr->right, clientName);
        }

    }
    /* Client has already previously occurred */
   return 0;
}

/*returns the Client struct if found returns NULL if not found*/
Client accessClient(Client *curr, char* clientName){
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
           returnVal = insertClient(curr->right, clientName);
        }

    }
    /* Client has already previously occurred */
   return curr;
}




/*For testing purposes only*/
void print(FILE *fp){

        /*Need ot add null check in case nothing has been added*/
        Node * lastWord = list->head;
        while(lastWord->right != NULL){
                lastWord = lastWord->right;
        }
        fprintf(fp, "{\"list\" :[  \n");
        printNode(list->head, fp, lastWord->token);

        fprintf(fp, "]}");
}



