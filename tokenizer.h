#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

struct TokenizerT_ {
	
	char *inputString;
	int currIndex;
	int inputSize;

}; typedef struct TokenizerT_ TokenizerT;


TokenizerT *TKCreate( char * ts );
void TKDestroy( TokenizerT * tk );
int isKeyWord(char *string);
int isDelimiter(char * c);
int buildHex(char * currentChar);
int isOp (char *currentChar);
int isComment(char * currentChar);
int hasNextToken(TokenizerT *tk);
char *TKGetNextToken( TokenizerT * tk );