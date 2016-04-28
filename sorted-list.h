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
        struct Client_ *left;
        struct Client_ *right;
}Client;

typedef struct Bank{
        Client *accounts;
        int numAccounts;
} Bank;

Client* createClient(char * clientName);
int addClient(char *clientName);
int insertClient(Client *curr, char* clientName);
Client* accessClient(Client *curr, char* clientName);





void print();
void printRecurs(Client* start);


 #endif



