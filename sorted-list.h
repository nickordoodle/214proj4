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

typedef struct Client{
        char *name;
        float balance;
        int inuse;
        int index;
        struct Client_ *left;
        struct Client_ *right;
}Client;

/*typedef struct Bank{
        Client *accounts;
        int numAccounts;
} Bank;
sorted-list has these as global variables
*/ 

Client* createClient(char * clientName);
int open(char *clientName);
int insertClient(Client *curr, char* clientName);
Client* start(Client *curr, char* clientName);





void print();
void printRecurs(Client* start);


 #endif



