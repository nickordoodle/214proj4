/*
 * tokenizer.c
 *
 * Nicholas Scherer, nds63
 * Griffin Wood,  gbw15
 */
 
#include "tokenizer.h"

/*To Do:
Do we need to deal with there being no char after a 0?
Need to add checks that malloc worked properly. In class he said we needed to do that.
Bad tokens must be printed out in hexform
*/


/*
 * Tokenizer type.  You need to fill in the type as part of your implementation.
 * Input String will contain the whole input. CurrentIndex is what position the program
 * is in while iterating through the string.  delimPositions will be an array of indexes
 * that refer to the indices where a delimiter exists in the inputString.
 *
 *  This implementation uses an inputString as the standard program argument, 
 * currIndex is how far through we are iterating through the string inputSize is the 
 * length of the inputString.
 */
 
/*
 * TKCreate creates a new TokenizerT object for a given token stream
 * (given as a string).
 * 
 * TKCreate should copy the arguments so that it is not dependent on
 * them staying immutable after returning.  (In the future, this may change
 * to increase efficiency.)
 *
 * If the function succeeds, it returns a non-NULL TokenizerT.
 * Else it returns NULL.
 *
 * First, check if string is empty or NULL.  If not, set all property values 
 * equal to 0 initially.  Allocate memory for pointer variables and make copies of variables,
 * then set them equal to the fields of the TokenizerT struct.  Return the struct.
 */

TokenizerT *TKCreate( char * ts ) {

	if( ts == NULL || ts[0] == '\0'){
		return 0;
	}
	
	/* Make copies of tokenizer and its properties, initialize all to 0 */
	char *inputString = 0;
	int currIndex = 0;
	int inputSize = 0;
	TokenizerT *tokenizer = 0;

	/* Allocate memory for the tokenizer and its fields */
	inputString = malloc(strlen(ts) + 1);
	tokenizer = malloc(sizeof(TokenizerT));

	/*  Copy the string paramter to inputString variable and get inputSize */
	strcpy(inputString, ts);	
	inputSize = strlen(inputString);
	

	/* Set new tokenizer field values */
	tokenizer->inputString = inputString;
	tokenizer->currIndex = currIndex;
	tokenizer->inputSize = inputSize;
	
	/* If successful, give a tokenizer with initialized fields */
	return tokenizer;
}

/*
 * TKDestroy destroys a TokenizerT object.  It should free all dynamically
 * allocated memory that is part of the object being destroyed.
 *
 * First, check if tokenizer is not null, then free all allocated fields of the 
 * TokenizerT struct which is only inputString. Then free the pointer to the struct itself.
 */

void TKDestroy( TokenizerT * tk ) {

	if(tk){
		free(tk->inputString);
		free(tk);
	}
		
}

/* After a word token is created, check to see if it matches a common keyword */
int isKeyWord(char *string){

	if(!strcmp(string, "if") || !strcmp(string, "else") || !strcmp(string, "do") 
	|| !strcmp(string, "while") || !strcmp(string, "return") || !strcmp(string, "switch") 
	|| !strcmp(string, "case") || !strcmp(string, "default") || !strcmp(string, "break") ){
	
		return 1;
	}
	return 0;
}

/* Check to see if current char is a delimiter and if so, skip it until end of input or 
    new token is found */
int isDelimiter(char * c){
        if(*c == ' '){
                return 1;
        }else if(*c == '\\'){
                if(*(c+1)== 't'
                        ||*(c+1)== 'v'
                        ||*(c+1)== 'f'
                        ||*(c+1)== 'n'
                        ||*(c+1)== 'r'){
                        return 2;
                }
        }
        return 0;
}

/* Function to increment currentchar in order to build it until the input no longer
    fits the hexidecimal token type */
int buildHex(char * currentChar){	

	int counter = 0;
	while(  (*currentChar >= '0' && *currentChar <= '9')  
				    ||
		(*currentChar >= 'A' && *currentChar <= 'F')
		                              ||	 
                 	(*currentChar >= 'a' && *currentChar <= 'f')   ){
			currentChar++;
			counter++;
	}

	return counter;
}

/* Executes checks for the C operators located on the reference sheet.  Some extra
*  checks are necessary for operators that have more than one character.  Returns 
* positive if operator and negative if bad token.  */

int isOp (char *currentChar){ 
	switch (*currentChar){
		case ' ':
			return 1;
		case '*':
			return 1;
		case '&':
			if(*(currentChar + 1) == '&'){
				currentChar++;
				return 2;
			}
			return 1;
		case '-':
			if(*(currentChar + 1) == '-'){
				currentChar++;
				return 2;
			}
			else if(*(currentChar + 1) == '>'){
				currentChar++;
				return 2;
			} else if(*(currentChar + 1) == '='){
				currentChar++;
				return 2;
			}
			return 1;
		case '!':
			if(*(currentChar + 1) == '='){
				currentChar++;
				return 2;
			}
			return 1;
		case '~':
			if(*(currentChar + 1) == '-'){
				currentChar++;
				return 2;
			}
			return 1;

		case '/':
			return 1;
		case '%':
			return 1;
		case '+':
			if(*(currentChar + 1) == '+'){
				currentChar++;
				return 2;
			} else if( *(currentChar + 1) == '='){
				currentChar++;
				return 2;
			}
			return 1;
		case '<':
			if(*(currentChar + 1) == '='){
				currentChar++;
				return 2;
			}else if( *(currentChar + 1) == '<'){
				currentChar++;
				return 2;
			}
			return 1;
		case '>':
			if( *(currentChar + 1) == '='){
				currentChar++;
				return 2;
			}else if( *(currentChar + 1) == '>'){
				currentChar++;
				return 2;
			}
			return 1;
		case '^':
			return 1;
		case '|':
			if( *(currentChar + 1) == '|'){
				currentChar++;
				return 2;
			}
			return 1;
		case '=':
			if( *(currentChar + 1) == '='){
				currentChar++;
				return 2;
			}
			return 1;
		case '(':
			return 1;
		case ')':
			return 1;
		case '[':
			return 1;
		case ']':
			return 1;
		case '{':
			return 1;
		case '}':
			return 1;
		case '.':
			return 1;
		case '?':
			return 1;
		case ':':
			return 1;
		default:
			return -1;
	}

	return 0;
}

/* Check to see if comments exist, if so, ignore input following the comments */
int isComment(char * currentChar){
        currentChar++;
        int count = 1;
        if(*currentChar == '\0')
                return 0;
        if(*currentChar == '/')
                return -1;
        else if(*currentChar == '*'){
                while(*currentChar != '\0'){
                        currentChar++;
                        count++;
                        if(*currentChar == '*'){
                                currentChar++;
                                count++;
                                if(*currentChar == '/'){
                                        count++;
                                        return count;
                                }
                        }
                }
        }
        return 0;

}
/* Helper function for main to see if we have iterated through entire input or not */

int hasNextToken(TokenizerT *tk){
	
	if(!tk || tk->inputString[tk->currIndex] == '\0'){
		return 0;
	} 
	
	return 1;

}


/*
 * TKGetNextToken returns the next token from the token stream as a
 * character string.  Space for the returned token should be dynamically
 * allocated.  The caller is responsible for freeing the space once it is
 * no longer needed.
 *
 * If the function succeeds, it returns a C string (delimited by '\0')
 * containing the token.  Else it returns 0.
 *
 * Algorithm: If first token is character/letter, then we have a word always. Then follow
 * an order of precedence to promote a "greedy" algorithm.  Order of precedence
 * is generally Word, Hexidecimal, Octal, Float, Decimal.
 * 
 
Build current token based on inputString and iterate
until delimiter found, new type, or end of array.
 * Algorithm: If first token is character/letter, then we have a word always. 
 * Word, Hex, Octal, Dec, Float
 */


	/*For all conditions, we use the start index which is where 'currIndex''
	left off from the last calls or 0 if it is the first call.  As the program iterates,
	currentChar increases and eventually stops.  We then take the substring
	of everything in between currentChar and startChar to get our new token.
 */

char *TKGetNextToken( TokenizerT * tk ) {

	
	char *inputString = tk->inputString;
	char *currentChar = &inputString[tk->currIndex];
	int counter = 0;
	char *startChar = currentChar;
	char *newToken = 0;
	int newTokenSize = 0;

	/* Ignore and skip any delimiters, they are not tokens */
	counter = isDelimiter(currentChar);
	while(counter != 0 ){
		currentChar+=counter;
		startChar+=counter;
		counter = 0;
		counter = isDelimiter(currentChar);
     }

	counter = 0;
	if(*currentChar == '\0');
        else if(*currentChar == '/'){
                counter = isComment(currentChar);
                if(counter == -1){
                        currentChar += strlen(currentChar);
                        startChar = currentChar;
                }
                if(counter > 0){
                        currentChar+=counter;
                        startChar+=counter;
                }else if(counter == 0){
                        currentChar++;
                }

        }

	/* Check for word */
	else if(isalpha(*currentChar)){
		/* Build word */
		while(isalpha(*currentChar) || isdigit(*currentChar)){
			currentChar++;
		}
	} 
	/* Check for Hex, Octal, Floats, and Decimals */
	else if( isdigit(*currentChar)){
		
		/* Octal or Hex */
		if(*currentChar == '0'){ 
		
			/* Found Hex */
			if( tolower(*(currentChar + 1)) == 'x') { 
				currentChar+=2;
				currentChar += buildHex(currentChar);
				
			} else { /* Build Octal/Decimal if not Hex */
				while( (*currentChar >= '0' && *currentChar <= '7') && (*currentChar != '\0') ){
					currentChar++;
				}

			}
		} else{/* Build Decimal unless period present, then build Float */
			int periodFound = 0;
			while(isdigit(*currentChar) && periodFound <= 1){
				if(*(currentChar + 1) == '.' &&
			                isdigit(*(currentChar + 2)) &&
			                !periodFound){
					periodFound++;
					currentChar+= 2;
				} else{
					currentChar++;
				}
			}
			
		}
	} else {
		counter = isOp(currentChar);
		if(counter > 0)
			currentChar+=counter;	
		else
			currentChar-=counter;
	}
	
	/* Calculate the token size using our char pointers */
	newTokenSize = strlen(startChar) - strlen(currentChar) + 1;
	/* Update tokenizerT's currIndex for the next tokens */
	tk->currIndex = strlen(inputString) - strlen(currentChar);
	
	/* Allocate memory for new token */
	newToken = malloc(sizeof(char) * newTokenSize);
	/* Copy substring of the input to get new token value */
	strncpy(newToken, startChar, newTokenSize - 1);
	
	if(isalpha(*newToken)){
	
		/* Add '\0' to end of array for delimiter */
		newToken[newTokenSize] = '\0';
	
		return newToken;
	}
	
	return NULL;
}


 /* 
 main will have a string argument (in argv[1]).
 * The string argument contains the tokens.
 * Print out the tokens in the second string in left-to-right order.
 * Each token should be printed on a separate line.
 */

