#ifndef SORTED_LIST_H
#define SORTED_LIST_H
/*
 *  * sorted-list.h
 *   */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* Used to act as our data for the list with pointers to its adjacent
 *    data Nodes */



typedef struct Record_{

        char *fileName;
        int occurrences;
        struct Record_ *next;
        struct Record_ *prev;
}Record;


//typedef struct Node_ Node;
typedef struct Node_{
        char *token;
        Record *head;
        struct Node_ *parent;
        struct Node_ *left;
        struct Node_ *right;

}Node;


typedef struct SortedList_{
        Node *head;
}SortedListPtr;



void recordRecord(Record *curr, char *filename);



void initializeSL();
int insertToken(Node *node, char *token, char *filename);
int SLInsert(char *token, char *filename);
void printNode(Node * curr, FILE *fp, char *lastNode);
void printRecord(Record * curr, FILE *fp);
void writeToFile(FILE *fp);
void basicFileName(char * name, char * revisedName);

Record * sortRecords(Record * head);
void destroyRecord(Record *record);



#endif
