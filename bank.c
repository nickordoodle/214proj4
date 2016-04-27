#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include <signal.h>

#include "sorted-list.h"

/* Only 20 clients can be serviced at one time
   and only one account to open at a time  */
pthread_mutex_t clientMutexes[20];
pthread_mutex_t newAccountMutex;

int main(int argc, char *argv[]){

	//This initializes the bank
	bank = newBank();
	numAccounts = 0;
	//The client acceptor thread listens for clients
	pthread_t clientListener;
	//The print thread prints the balances every 20 seconds
	pthread_t printStatus;
	//This signal handler will wait for the server to receive SIGINT so it can shut the clients down
	signal(SIGINT, sigint_handler);
	//Start the client acceptor thread
	if(pthread_create(&clientListener, 0, clientListenerThread, 0) != 0){
		printf("ERROR: Could not start server.\n");
		exit(EXIT_FAILURE);
	}
	//Start the printing thread
	pthread_create(&printStatus, 0, printStatusThread, 0);
	pthread_join(clientListener, NULL);
	
	printf("SERVER: Server end.\n");
	return 0;
}

/* Utility thread to print account balances
   every 20 seconds */
void* printStatusThread(void* arg){
	
	while(1){

		printf("SERVER:\nCurrent balances:\n");
		pthread_mutex_lock(&newAccountMutex);

		/* TO IMPLEMENT: PRINT STATUS OF ALL ACCOUNTS HERE */

		pthread_mutex_unlock(&newAccountMutex);

		/* Control this output to every 20 seconds */
		sleep(20);
	}
}

/* This thread will handle incoming requests from new 
   clients so that they can be added to start operations
   or be told to wait */
void* clientListenerThread(){


	/* Needs to listen over a socket,
	   and accept a connection, then start
	   a new client session thread */

}


/* This will essentially be our customer session thread/function
   which handles all of the customer operations */
void* clientSessionThread(){


	/* Compares all the command operations
	   and performs them appropriately */

}
