#include "sorted-list.h"

SortedListPtr *list;

/* Allocate memory, set data fields to appropriate values*/
void initializeSL(){

        list = (SortedListPtr*) malloc(sizeof(struct SortedList*));


        list->head = NULL;
        return;
}

/*current file will be the most recent so you only need to check there since each file is only opened once*/
void recordRecord(Record *curr, char *filename){
        if(strcmp(curr->fileName, filename) == 0){
                curr->occurrences++;
        }else if(curr->next ==NULL){
                /*Add to end of unsorted list*/
                Record *newRecord = malloc(sizeof(Record));

                newRecord ->fileName = malloc(sizeof(char) * strlen(filename));
                strcpy(newRecord -> fileName, filename);

                newRecord->occurrences = 1;
                newRecord->prev = curr;
                newRecord->next = NULL;
                curr->next = newRecord;
        }/*check next Record*/
        else
                recordRecord(curr->next, filename);

        return;
}



Node *createNode(Node *parent, char *token, char *filename){

        Record *record = malloc(sizeof(Record));
        Node *newNode = malloc(sizeof(Node));

        record ->fileName = malloc(sizeof(char) * strlen(filename));
        strcpy(record -> fileName, filename);
        record->occurrences = 1;
        record->next = NULL;
        record->prev = NULL;

        newNode ->token = malloc(sizeof(char) * strlen(token));

        strcpy(newNode -> token, token);
        newNode->head = record;
        newNode->parent = parent;
        newNode->left = NULL;
        newNode->right = NULL;

        return newNode;
}
/*What is the purpose of parent here?*/
int insertToken(Node *node, char *token, char *filename){

        void *nodeData = node->token;
        int compareVal = strcmp(token, nodeData);


        int returnVal = 0;


    /* Insert into left subtree */
    if(compareVal < 0){
        if(node -> left  == NULL){
                node -> left = createNode(node, token, filename);
                returnVal = 1;
        } else{
            returnVal = insertToken(node->left, token, filename);

        }

    }
    /* Insert into right subtree */
    else if (compareVal > 0){
        if(node -> right == NULL){
                node -> right = createNode(node, token, filename);
                                                                             returnVal = 1;
        } else{
           returnVal = insertToken(node->right,token, filename);

        }

    }
    /* Word has already previously occurred and records need to be updated */
    else {
        recordRecord(node->head,filename);
        returnVal = 0;
    }

    return returnVal;
}



/*   Checks for null BST and then send it to inserttoken to update the token information*/

int SLInsert(char *token, char *filename){

        int returnVal = 0;
        int i = 0;
        
        char * revisedName = malloc(strlen(filename)); 
        basicFileName(filename, revisedName);

        for(i = 0; i < strlen(token); i++)
                *(token + i) = tolower(*(token + i));

        /* Empty tree, make new one */
        if(list == NULL){
                initializeSL();
                list->head = createNode(NULL, token, revisedName);
                return 1;
        }

        returnVal = insertToken(list->head, token, revisedName);


        return returnVal;
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

/* The skeleton of the records to be printed */
void printNode(Node * curr, FILE *fp, char * lastWord){

        if(curr==NULL){
                return;
        }
        printNode(curr->left, fp, lastWord);


        fprintf(fp, "\t{\"%s\" :[  \n", curr->token);

        curr->head = sortRecords(curr->head);
        /* Print All filenames and occurrences here */
        printRecord(curr->head, fp);

        if(curr->token == lastWord)
                fprintf(fp, "\t]}\n");
        else
            fprintf(fp, "\t]},\n");

        printNode(curr->right, fp, lastWord);

        return;
}

/* This function is where we actually output the contents of each "record" */
void printRecord(Record * curr, FILE *fp){
        if(curr==NULL){
                return;
        }

        fprintf(fp, "\t\t{\"%s\" : %d}", curr->fileName, curr->occurrences);

        if(curr->next != NULL)
                fprintf(fp, ",\n");
        else
                fprintf(fp, "\n");

        printRecord(curr->next, fp);

        return;

}

/* Used to maintain descending order of records (highest to lowest) in each linked list if it has the same number of occurences its sorted alphabetically*/
Record * sortRecords(Record * head){

    Record *temp;
    Record *sortedList;
    Record *curr;
    int compareVal = 0;


    sortedList = head;
    head = head->next;

    sortedList->next = NULL;
    temp = sortedList;

    while(head != NULL){
    /*check to see if head should be added to the front of the new list*/
            if(head->occurrences >= sortedList->occurrences){
                compareVal = strcmp(head->fileName,sortedList->fileName);
                if(compareVal <= 0 || head->occurrences == sortedList->occurrences){
                    temp = head->next;
                    head->next = sortedList;

                    sortedList = head;
                    head = temp;
                    continue;
                }
            }

    /*Check to see where after the front head should be added to the new list*/
      curr = sortedList;
          while(curr->next != NULL){
                if( head->occurrences > curr->next->occurrences)
                            break;
                if(head->occurrences == curr->next->occurrences){
                        while(curr->next != NULL)
                                if(head->occurrences == curr->next->occurrences){
                                        compareVal = strcmp(head->fileName,curr->next->fileName);
                                if(compareVal <= 0)
                                        break;
                                curr = curr->next;
                        }
                        break;
                }

                curr = curr->next;
            }

            if(curr->next != NULL)
                    curr->next->prev = head;
            temp = head->next;
            head->next = curr->next;
            curr->next = head;
            head->prev = curr;
            head = temp;
    }

    return sortedList;
}



int countTokens(Node *head, int count){

    if(head == NULL)
        return count;

    Record *curr = head->head;

    do{

        count += curr->occurrences;
        curr = curr->next;

    } while(!curr);

    return countTokens(head->left, count);
    return countTokens(head->right, count);

}

void basicFileName(char * name, char * revisedName){
        int i = 0;
        int foundSlash = 0;

        for(i = 0; i < strlen(name); i++)
                if(*(name + i) == '/'){
                    foundSlash = i + 1;
                }
        strcpy(revisedName, name + foundSlash);

        return;
}

