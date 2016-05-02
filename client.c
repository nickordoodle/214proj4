#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <signal.h>
#include <ctype.h>

#include "network.h"
#include "client.h"

#define COMMAND_SIZE 110


/* Vars used for commands, responses and socket numer */
int sockfd;
char serverBuff[508]; 
char commandBuff[508];

void error(char *msg){
    perror(msg);
    exit(0);
}


/* A 'listener' thread for waiting for the user to pick
   a command to execute according to what he or she wants to do
   */
void *userCommandThread(void *input){

	/* Loop through user commands and listen
	   for valid command to execute */

	int sockfd = *(int *)input;

	printf("Please enter a command from the following:\n");
	printf("[open] account name, [start] account name [credit] amount, [debit] amount, [balance], [finish], [exit]\n");

	while(1){

		char comm[COMMAND_SIZE];
		memset(comm, '\0', COMMAND_SIZE);
		
		memset(serverBuff, '\0', strlen(serverBuff));
		

		/* Get the user input */
		fgets(comm, COMMAND_SIZE, stdin);
		
		if ((strlen(comm)>0)){
			comm[strlen (comm) - 1] = '\0';
		}
		
		if(!isValidCommand(comm)){
			printf("Not a valid command, please try again.\n");
			continue;
		}
		
		/* Write the user command to the serverBuffer */
		strcpy(serverBuff, comm);
		
		//Send the serverBuffer to the server
		if ((send(sockfd, serverBuff, strlen(serverBuff),0))== -1) {
                printf("ERROR: Could not send message.\n");
                close(sockfd);
                exit(1);
        }
        
        /* Have the user command input sleep for two seconds */
        sleep(2);
	}

	return NULL;
}

int isValidCommand(char *command){

	/* Check if first argument is valid */
	if(!strcmp(command, "finish") || !strcmp(command, "exit") )
		return 1;
	else if(!strncmp(command, "open", 4) || !strncmp(command, "balance", 7)
		|| !strncmp(command, "start", 5) 
		|| !strncmp(command, "credit", 6) || !strncmp(command, "debit", 5) )
		return 1;


	return 0;
}


/* A thread for taking the user command and executing it on the server side.
   This thread executes what the user wants and sends back the appropriate
   message depending on the bank/account operation being done */
void *serverResponseThread(void *input){

	int sockfd = *(int *)input;
	while(1){
		memset(serverBuff, '\0', strlen(serverBuff));
		int num = recv(sockfd, serverBuff, sizeof(serverBuff), 0);
		if(num <= 0){
			printf("ERROR: Connection closed.\n");
			break;
		}
		
		if(strcmp(serverBuff, "end") == 0){
			printf("Client closing.\n");
			exit(0);
		}

		printf("SERVER REPONSE: %s \n", serverBuff);
		
	}

	return NULL;	
}


void sigint_handler(int sig){
	/* Clear out our server commands */
	memset(serverBuff, '\0', strlen(serverBuff));

	/* We want to stop the server so send finish command to 
	   the server */
	strcpy(serverBuff, "finish");
	if ((send(sockfd,serverBuff, strlen(serverBuff),0))== -1) {
		printf("ERROR: Could not close client account session on exit.\n");
		close(sockfd);
		exit(1);
    }
	exit(0);
}


int main(int argc, char *argv[])
{
	// Declare initial vars
    	// file descriptor for our socket
	int portno = -1;	// server port to connect to
	int n = -1;			// utility variables - for monitoring reading/writing from/to the socket
	sockfd = -1; 		
    struct sockaddr_in serverAddressInfo;	// Super-special secret C struct that holds address info for building our socket
    struct hostent *serverIPAddress;	// Super-special secret C struct that holds info about a machine's address
    
	
	//Thread to get server message
	pthread_t serverResponse;
	//Command input waits for input from the user and sends it to the client
	pthread_t userCommand;
	
	// If the user didn't enter enough arguments, complain and exit
    if (argc  == 2)
	{

		// convert the text representation of the port number given by the user to an int
		portno = MYPORT;

    } else if(argc == 3){
    	portno = atoi(argv[2]);

    } else{
		fprintf(stderr,"usage 1: %s [hostname] [port] for user designated port\n", argv[0]);
		fprintf(stderr,"usage 2: [hostname] for default port\n");
		exit(0);
    }

	
	// look up the IP address that matches up with the name given - the name given might
	//    BE an IP address, which is fine, and store it in the 'serverIPAddress' struct
    serverIPAddress = gethostbyname(argv[1]);
    if (serverIPAddress == NULL)
	{
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
				
	// try to build a socket .. if it doesn't work, complain and exit
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
	{
        error("ERROR creating socket");
	}


	
	/** We now have the IP address and port to connect to on the server, we have to get    **/
	/**   that information into C's special address struct for connecting sockets                     **/

	// zero out the socket address info struct .. always initialize!
    bzero((char *) &serverAddressInfo, sizeof(serverAddressInfo));

	// set a flag to indicate the type of network address we'll be using 
    serverAddressInfo.sin_family = AF_INET;
	
	// set the remote port .. translate from a 'normal' int to a super-special 'network-port-int'
	serverAddressInfo.sin_port = htons(portno);

	// do a raw copy of the bytes that represent the server's IP address in 
	//   the 'serverIPAddress' struct into our serverIPAddressInfo struct
    bcopy((char *)serverIPAddress->h_addr, (char *)&serverAddressInfo.sin_addr.s_addr, serverIPAddress->h_length);



	/** We now have a blank socket and a fully parameterized address info struct .. time to connect **/
	
	// try to connect to the server using our blank socket and the address info struct 
	//   if it doesn't work, complain and exit
    while(connect(sockfd, (struct sockaddr *)&serverAddressInfo, sizeof(serverAddressInfo))<0){
		printf("Establishing connection with server...\n");	
		sleep(3);	
	}	

	// zero out the message serverBuffer
    bzero(serverBuff,256);


	// try to write it out to the server
	n = write(sockfd,serverBuff,strlen(serverBuff));
	
	// if we couldn't write to the server for some reason, complain and exit
    if (n < 0)
	{
         error("ERROR writing to socket");
    }
	
	// sent message to the server, zero the serverBuffer back out to read the server's response
	bzero(serverBuff,256);

	// if we couldn't read from the server for some reason, complain and exit
    if ((n = read(sockfd,serverBuff,255)) < 0)
	{
         error("ERROR reading from socket");
	} else{
		printf("%s\n", serverBuff );
	}



	/* INSERT CLIENT COMMAND THREAD AND SERVER RESPONSE THREAD HERE */


	//Sets up a signal handler to finish account sessions on exit
	signal(SIGINT, sigint_handler);
	
	if(pthread_create(&serverResponse, 0, serverResponseThread, &sockfd) != 0){
		printf("ERROR: Failure launching response output thread.\n");
		exit(1);
	}
	
	if(pthread_create(&userCommand, 0, userCommandThread, &sockfd) != 0){
		printf("ERROR: Failure launching command input thread.\n");
		exit(1);
	}

	pthread_join(userCommand, NULL);
	printf("Client end.\n");


	// print out server's message
    printf("%s\n",serverBuff);


    return 0;
}
