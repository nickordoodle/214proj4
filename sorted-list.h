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

typedef struct Client_{
        char *name;
        float balance;
        int inuse;
        int index;
        struct Client_ *left;
        struct Client_ *right;
}Client;


Client* createClient(char * clientName, int clientCount);
int insertClient(Client *curr,int clientCount, char* clientName);
Client* start(Client *curr, int clientCount, char* clientName);






void print(Client* head);
void printRecurs(Client* start);


 #endif



