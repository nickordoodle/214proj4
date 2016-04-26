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
        /*client already exsists*/
      
        void *nodeData = node->token;
        int compareVal = strcmp(curr->name, clientName);

    /* Insert into left subtree */
    if(compareVal < 0){
        if(curr -> left  == NULL){
                node -> left = createClient(clientName);
                return 1;
        } else{
            return insertClient(curr->left, token, clientName);
        }
    }
    
    /* Insert into right subtree */
    else if (compareVal > 0){
        if(curr -> right == NULL){
                curr -> right = createClient(clientName);
        } else{
           returnVal = insertClient(curr->right,token, clientName);
        }

    }
    /* Client has already previously occurred */
   return 0;
}




/*this will be turned into how we get the information out of the BST not done yet*/
void writeToFile(FILE *fp){

        /*Need ot add null check in case nothing has been added*/
        Node * lastWord = list->head;
        while(lastWord->right != NULL){
                lastWord = lastWord->right;
        }
        fprintf(fp, "{\"list\" :[  \n");
        printNode(list->head, fp, lastWord->token);

        fprintf(fp, "]}");
}



