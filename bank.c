#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include <signal.h>
#include <sys/mman.h>


#include "bank.h"
#include "client.h"
#include "network.h"

/* Only 20 clients can be serviced at one time
   and only one account to open at a time  */


Map * globalVar = NULL;
int currAccount = -1;

int main(int argc, char *argv[]){

	globalVar =(Map *) mmap(NULL, sizeof(Map), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANON,0,0);
	globalVar->accountCount = 0;

    int index = 0;
    while(index < 20){
        globalVar->name[index] = (char *) mmap(NULL, sizeof(char) * 110, PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANON,0,0);
        memset(globalVar->name[index], '\0', strlen(globalVar->name[index]));

        index++;
    }
    memset((void *)globalVar->processes, 0, 20*sizeof(pid_t));
	/*The client acceptor thread listens for clients*/
	pthread_t clientListener;
	/*The print thread prints the balances every 20 seconds*/
	pthread_t printStatus;
	

	/*This signal handler will wait for the server to receive SIGINT so it can shut the clients down*/
	signal(SIGINT, signalHandler);
	


	/*Start the client acceptor thread*/
	if(pthread_create(&clientListener, 0, clientListenerThread, 0) != 0){
		printf("ERROR: Could not start server.\n");
		exit(EXIT_FAILURE);
	}
	/*Start the printing thread*/
	pthread_create(&printStatus, 0, printStatusThread, 0);
	pthread_join(clientListener, NULL);
	
    wait(NULL);

	printf("SERVER: Server end.\n");
	return 0;
}

/* Utility thread to print account balances
   every 20 seconds */
void* printStatusThread(void* arg){
	
	while(1){

		printf("SERVER:\nCurrent balances:\n");
		pthread_mutex_lock(&globalVar->newAccountMutex);

        print();


		pthread_mutex_unlock(&globalVar->newAccountMutex);

		/* Control this output to every 20 seconds */
		sleep(20);
	}
}

void signalHandler(){

    int index = 0;
    while(index < 20){

        if(globalVar->processes[index] != 0){
            kill(globalVar->processes[index], SIGTERM);

        }

        index++;
    }

    exit(0);
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

        if(newClientSock > 0){

            printf("\nA new client has connected.\n");
            pid_t pid = fork();

            int index = 0;
            while(globalVar->processes[index] != 0){
                index++;
            }
            globalVar->processes[index] = pid;

            if(pid < 0){

                char *message = "SERVER: Could not create child process.";
                printf("SERVER: %s\n", message);
                write(newClientSock, message, strlen(message));

            } else if(pid == 0){

                /* This is where the child will execute code */
                printf("Created a child process for a new client.  The process ID is: %d\n", getpid());
                clientSession(newClientSock);

            } 


        }
		
		

	/*	if(pthread_create(&clientSession, 0, clientSessionThread, &newClientSock) ){
			printf("ERROR: Could not launch client session thread.\n");
			sleep(1);
		}*/	

	}

	return NULL;
}
/*
void childprocess(int arg){
	//thread read 
	clientSession(int arg);
} */


/* This will essentially be our customer session function
   which handles all of the customer operations */
void clientSession(int arg){
	/*Shoudlnt it be command + extra <= 100?*/
	int sockfd = arg;
	char clientCommand[100];
	char firstArg[100];
	char secondArg[100];
	/* Compares all the command operations
	   and performs them appropriately */

	while(1){

        memset(clientCommand, '\0', strlen(clientCommand));
        memset(firstArg, '\0', strlen(firstArg));
        memset(secondArg, '\0', strlen(secondArg));


		int value = recv(sockfd, clientCommand, sizeof(clientCommand), 0);


                /*
		if (value == -1) { 
			printf("ERROR: Could not receive data from client.\n");
			break;

		} else*/
         if (value == 0) {
			printf("SERVER: Connection closed with client.\n");
			break;
		} else if(value > 0){

            sscanf(clientCommand, "%s %s", firstArg, secondArg);
            int index = 0;
            while(isalpha(firstArg[index])){
                index++;
            }

            firstArg[index] = '\0';

            index = 0;
            while(isalpha(secondArg[index]) || isdigit(secondArg[index])){
                index++;
            }

            secondArg[index] = '\0';

            handleUserCommands(firstArg, secondArg, sockfd);
        }


	}

	return;
}


void print(){
        int i = 0;
        for(i = 0; i <  globalVar->accountCount; i++){
                if(globalVar->inuse[i] == 0)
                        printf("Client:%s: \n\tBalance:%.2f\n\tIn use: no\n",globalVar->name[i],globalVar->balance[i]);
                else
                        printf("Client:%s: \n\tBalance:%.2f\n\tIn use: yes\n",globalVar->name[i],globalVar->balance[i]);

        }
}

void openfnc(char * clientMsg, char *acc){

        if(currAccount >= 0){
                sprintf(clientMsg, "Unable to open account while in session");
                return;
        }

        int result = -1;
	int i = 0;
	if(globalVar->accountCount == 20){
		sprintf(clientMsg, "Unable to open new account: Account limit reached\n");

	}

    pthread_mutex_lock(&globalVar->newAccountMutex);

    for(i = 0; i < 20; i++){
            result = strcmp(globalVar->name[i],acc);
            if(result == 0){
                    sprintf(clientMsg, "Unable to open new account: Account name already in use\n");
                    return;
            }
    }

    strcpy(globalVar->name[(globalVar->accountCount)],acc);
    sprintf(clientMsg, "Account successfully opened\n");
    globalVar->accountCount++;

    

    pthread_mutex_unlock(&globalVar->newAccountMutex);

    return;
}

void startfnc(char * clientMsg, char* acc){
    int i = 0;
    if(currAccount >= 0){
            sprintf(clientMsg, "Unable to open start a second session.");
            return;
    }
	 
    for(i = 0; i < 20; i++){
            result = strcmp(globalVar->name[i],acc);
            if(result == 0){
            	currAccount == i;
                    break;
            }
    }
    

 if(currAccount < 0)
                sprintf(clientMsg, "Unable to open account: invalid account name");
        else
                sprintf(clientMsg, "Session for account %s successfully started",globalVar->name[currAccount]);

        if(pthread_mutex_trylock(&globalVar->clientMutexes[currAccount]) != 0)
                sprintf(clientMsg, "ERROR: This account is already in session elsewhere.");

    else 
        sprintf(clientMsg, "Account %s successfully opened",acc);
    

                
        
        
}

void credit(char * clientMsg, char* num){
	int badentry = 0;
	int i;
	for(i = 0; i < strlen(num);i++)
		if(!isdigit(*(num+i)))
			badentry = 1;
	
	if(badentry != 0){
		sprintf(clientMsg, "Must enter a number after command [credit].");
		return;
	}
         if(currAccount >= 0){
                float change = atof(num);
                globalVar->balance[currAccount] += change;
                sprintf(clientMsg, "Balance has been updated.");

                return;
        }
        sprintf(clientMsg, "Command [credit] can only be done after an account session has started.");

        return;
}
void debit(char * clientMsg, char* num){
	int badentry = 0;
	int i;
	for(i = 0; i < strlen(num);i++)
		if(!isdigit(*(num+i)))
			badentry = 1;
	
	if(badentry != 0){
		sprintf(clientMsg, "Must enter a number after command [debit].");
		return;
	}
	if(currAccount >= 0){
                float change = atof(num);
                if(globalVar->balance[currAccount] >= change){
                        globalVar->balance[currAccount] -= change;

                        sprintf(clientMsg, "Balance has been updated.");
                }else
                        sprintf(clientMsg, "Unable to complete transaction: Debit larger than balance.");
                return;
        }
        sprintf(clientMsg, "Command [debit] can only be done after an account session has started.");

        return;
}


void balance(char * clientMsg){
 	if(currAccount >= 0){
                sprintf(clientMsg, "Current Balance: %f",  globalVar->balance[currAccount]);

                return;
        }
        sprintf(clientMsg, "Command [balance] can only be done after an account session has started.");

        return;
}


void finish(char * clientMsg){

        if( globalVar->balance[currAccount] >= 0){
                 globalVar->inuse[currAccount] = 0;

                pthread_mutex_unlock(&globalVar->clientMutexes[currAccount]);

                currAccount = -1;

                sprintf(clientMsg, "Session ended.");

                return;
        }
        sprintf(clientMsg, "Command [finish] can only be done after an account session has started.");

        return;
}




void exitClient(char * clientMsg){

    sprintf(clientMsg, "end");     


}


/* This is where the data handling comes in */
void handleUserCommands(char *command, char *accOrNum, int sockfd){

        char clientMsg[100];

        memset(clientMsg, '\0', strlen(clientMsg));

        if(!strcmp(command, "open")){
                
            /*Will utilize the open account mutex and attempt to open an account*/

            openfnc(clientMsg, accOrNum);

        } else if(!strcmp(command, "start")){
                
            /*Starts a 'customer session' for the user*/
            startfnc(clientMsg, accOrNum);


        } else if(!strcmp(command, "credit")){
                
            credit(clientMsg, accOrNum);

        } else if(!strcmp(command, "debit")){
                
            debit(clientMsg, accOrNum);

        } else if(!strcmp(command, "balance")){
                
            balance(clientMsg);

        } else if(!strcmp(command, "finish")){
	           
           finish(clientMsg);
        
        } else if(!strcmp(command, "exit")){

            /*Disconnects the client from the server and ends the client process*/
            exitClient(clientMsg);

        }


        /* Send back the client a message of what happened */
        write(sockfd, clientMsg, strlen(clientMsg));

}
