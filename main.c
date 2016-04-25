#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"


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

	while(1){

		printf("Please enter a command from the following:\n");
		printf("[open] accountname, [start] accountname [credit] amount,
			[debit] amountbalance, [finish], [exit]\n");

		
		char command[MAX_COMMAND_SIZE];
		memset(command, '\0', MAX_COMMAND_SIZE);
		
		memset(serverBuff, '\0', strlen(serverBuff));
		
		//These are used in extracting the operation and argument from the users input
		char op[100];
		char arg[100];
		char err[100];
		fgets(command, MAX_COMMAND_SIZE, stdin);
		
		if ((strlen(command)>0) && (command[strlen (command) - 1] == '\n')){
			command[strlen (command) - 1] = '\0';
		}
		
		if(!isValidCommand)
			continue;
		
		//Write the user command to the serverBuffer
		strcpy(serverBuff, op);
		
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
	else if(!strncmp(command, "open", 4) || !strncmp(command, "start", 5) 
		|| !strncmp(command, "credit", 6) || !strncmp(command, "debit", 5) )
		return 1;


	return 0;
}


/* A thread for taking the user command and executing it on the server side.
   This thread executes what the user wants and sends back the appropriate
   message depending on the bank/account operation being done */
void *serverResponseThread(void *input){


	return NULL;	
}


void sigint_handler(int sig){
	memset(serverBuff, '\0', strlen(serverBuff));
	strcpy(serverBuff, "finish");
	if ((send(sockfd,serverBuff, strlen(serverBuff),0))== -1) {
		printf("ERROR: Could not close client account session on exit.\n");
		close(sockfd);
		exit(1);
    }
	exit(0);
}

/* Vars used for commands, responses and socket numer */
int sockfd;
char serverBuff[508]; 
char commandBuff[508];

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
    if (argc < 3)
	{
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }
	
	
	
	/** If the user gave enough arguments, try to use them to get a port number and address **/

	// convert the text representation of the port number given by the user to an int
	portno = atoi(argv[2]);
	
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

	
	/** If we're here, we're connected to the server  **/
	
    printf("Please enter the message: ");
	
	// zero out the message serverBuffer
    bzero(serverBuffer,256);

	// get a message from the client
    fgets(serverBuffer,255,stdin);
    
	// try to write it out to the server
	n = write(sockfd,serverBuffer,strlen(serverBuffer));
	
	// if we couldn't write to the server for some reason, complain and exit
    if (n < 0)
	{
         error("ERROR writing to socket");
    }
	
	// sent message to the server, zero the serverBuffer back out to read the server's response
	bzero(serverBuffer,256);

	// read a message from the server into the serverBuffer
    n = read(sockfd,serverBuffer,255);
	
	// if we couldn't read from the server for some reason, complain and exit
    if (n < 0)
	{
         error("ERROR reading from socket");
	}



	/* INSERT CLIENT COMMAND THREAD AND SERVER RESPONSE THREAD HERE */


	//Sets up a signal handler to finish account sessions on exit
	signal(SIGINT, sigint_handler);
	

	//TODO: IMPLEMENT THREAD FUNCTIONS
	if(pthread_create(&userCommand, 0, userCommandThread, &sockfd) != 0){
		printf("ERROR: Failure launching command input thread.\n");
		exit(1);
	}
	if(pthread_create(&serverResponse, 0, serverResponseThread, &sockfd) != 0){
		printf("ERROR: Failure launching response output thread.\n");
		exit(1);
	}
	
	pthread_join(user_command, NULL);
	printf("Client end.\n");


	// print out server's message
    printf("%s\n",serverBuffer);


    return 0;
}
