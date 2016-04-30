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
#include "bank.h"
#include "client.h"
#include "network.h"

/* Only 20 clients can be serviced at one time
   and only one account to open at a time  */
pthread_mutex_t clientMutexes[20];
pthread_mutex_t newAccountMutex;

Client currAccount = NULL;

int main(int argc, char *argv[]){


	/*The client acceptor thread listens for clients*/
	pthread_t clientListener;
	/*The print thread prints the balances every 20 seconds*/
	pthread_t printStatus;
	

	/*This signal handler will wait for the server to receive SIGINT so it can shut the clients down*/
	/*signal(SIGINT, sigint_handler);*/
	


	/*Start the client acceptor thread*/
	if(pthread_create(&clientListener, 0, clientListenerThread, 0) != 0){
		printf("ERROR: Could not start server.\n");
		exit(EXIT_FAILURE);
	}
	/*Start the printing thread*/
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
void* clientListenerThread(void *arg){


	/* Needs to listen over a socket,
	   and accept a connection, then start
	   a new client session thread */
   	int sockfd = 0;
   	int newClientSock = 0;
   	char outputMsg[100];
   	struct sockaddr_in serverAddressInfo;
        pid_t  pid;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	serverAddressInfo.sin_family = AF_INET;
	serverAddressInfo.sin_addr.s_addr = htonl(INADDR_ANY);
	serverAddressInfo.sin_port = htons(MYPORT);

	/* Bind socket to address */
	bind(sockfd, (struct sockaddr*)&serverAddressInfo, sizeof(serverAddressInfo));


	while(1){

		if(listen(sockfd, 4) < 0){
			printf("SERVER: A listen error occurred.");
		}

	    newClientSock = accept(sockfd, (struct sockaddr*)NULL, NULL);
    	strcpy(outputMsg, "SERVER: Connection to server successful.\n");
		printf("SERVER: Connection to client successful.\n");
		write(newClientSock, outputMsg, strlen(outputMsg));

		/* The thread to be created for new clients */
		fork();
		if(pid == 0)
			clientSession();
		/* potentially should store */	
		

	/*	if(pthread_create(&clientSession, 0, clientSessionThread, &newClientSock) ){
			printf("ERROR: Could not launch client session thread.\n");
			sleep(1);
		}*/	

	}

	return NULL;
}


/* This will essentially be our customer session function
   which handles all of the customer operations */
void clientSession(){
	/*Shoudlnt it be command + extra <= 100?*/
	int sockfd = *(int *)arg;
	char clientCommand[100];
	char firstArg[100];
	char secondArg[100];
	/* Compares all the command operations
	   and performs them appropriately */

	while(1){

		int value = recv(sockfd, clientCommand, sizeof(clientCommand), 0);

		if (value == -1) { 
			printf("ERROR: Could not receive data from client.\n");
			break;
		} else if (value == 0) {
			printf("SERVER: Connection closed with client.\n");
			break;
		}

		sscanf(clientCommand, "%s %s", firstArg, secondArg);
		/*if an ac*/
		if(currAccount == NULL)
			handleClientCommands(firstArg, secondArg, sockfd);
		else
			handleCustomerCommands(firstArg, secondArg, sockfd);
	}

	return NULL;
}

/* This is where the data handling comes in */
void handleClientCommards(char *command, char *accOrNum, int sockfd){
	char clientMsg[100];


	
	memset(clientMsg, '\0', strlen(clientMsg));
	
	
	
	if(strcmp(command, "open")){
		/*Will utilize the open account mutex and attempt to open an account*/
		int result = -1;

		pthread_mutex_lock(&newAccountMutex);
 

		result = open(accOrNum);
		
		else if(result == 0)
			sprintf(clientMsg, "Account successfully opened\n");
		else if(result == 1)
			sprintf(clientMsg, "Unable to open new account: Account limit reached\n");
		else if(result == 2)
			sprintf(clientMsg, "Unable to open new account: Account name already in use\n");
		else
			sprintf(clientMsg, "Function open has failed\n");
		
		pthread_mutex_unlock(&newAccountMutex);

	} else if(strcmp(command, "start")){
		/*Starts a 'customer session' for the user*/
		currAccount = start(accOrNum);
		
		if(currAccount == NULL)
			sprintf(clientMsg, "Unable to open account: invalid account name");
		else
			sprintf(clientMsg, "Account %s successfully opened",accOrNum);
		
		if(pthread_mutex_trylock(&clientMutexes[currAccount->index]) != 0){
			sprintf(clientMsg, "ERROR: This account is already in session elsewhere.");
			return;	
		}
		
	} else if(strcmp(command, "credit") || strcmp(command, "debit") || (strcmp(command, "balance") || strcmp(command, "finish")){
		sprintf(clientMsg, "Command %s can only be done after an account session has started.", command);
	} else if(strcmp(command, "exit")){
		
		/*Disconnects the client from the server and ends the client process*/


	}


	/* Send back the client a message of what happened */
	write(sockfd, clientMsg, strlen(clientMsg));

}

void handleCustomerCommards(char *command, char *accOrNum, int sockfd){

	char clientMsg[100];
	memset(clientMsg, '\0', strlen(clientMsg));
	

	if(strcmp(command, "open")){
		sprintf(clientMsg, "Unable to open account while in session");
	} else if(strcmp(command, "start")){
		sprintf(clientMsg, "Unable to open start a second session.");
	} else if(strcmp(command, "credit")){
		float change = atof(accOrNum);
		currAccount->balance += change;
		sprintf(clientMsg, "Balance has been updated.");
	} else if(strcmp(command, "debit")){
		float change = atof(accOrNum);
		if(currAccount->balance >= change){
			currAccount->balance -= change;
			sprintf(clientMsg, "Balance has been updated.");
		}else
			sprintf(clientMsg, "Unable to complete transaction: Debit larger than balance.");
	} else if(strcmp(command, "balance")){
		sprintf(clientMsg, "Current Balance: %f", currAccount->balance);
	} else if(strcmp(command, "finish")){
		int index = currAccount->index;
		currAccount->inuse = 0;
		currAccount = NULL;
		pthread_mutex_unlock(&clientMutexes[index]);
		sprintf(clientMsg, "Session ended.");

	} else if(strcmp(command, "exit")){
		/*Disconnects the client from the server and ends the client process*/
		


	}


	/* Send back the client a message of what happened */
	write(sockfd, clientMsg, strlen(clientMsg));

}
