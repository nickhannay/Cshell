#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>

#include "built-in.h"



/* This function reads BUFFER_SIZE bytes from the file pointed to by <fp> into <input>
 *
 * Interactive mode - fp = stdin
 * Script mode - fp = FILE* returned by fopen in main
 *
 * parses <input> by " " into the array <args>
 * args[0] is the command and args[1] - args[N] are the supplied arguments
 * args is NULL terminated and is passed directly to execvp()
 * returns args[0](command) or NULL when an empty line is read in script mode
 */
char* formatInput(char* input , char* args[]){
	// remove trailing \n
	input[strcspn(input, "\n")] = '\0';
	if( strlen(input) == 0){
		args[0] = "";
		args[1] = NULL;

	}
	else{
        if (strchr(input, ' ') == NULL){
            args[0] = input;
            args[1] = NULL;
        }
        else{
            args[0] = strtok(input, " ");

    		char* token;
    		int i = 1;
    		while((token = strtok(NULL, " ")) != NULL){
    			args[i] = token;
    			i++;
    		}
            args[i] = NULL;
        }

	}
  	return args[0];
}


/*
 * args does not capture inputted spaces
 * returns 1 if there was an error involving environment variables (i.e. unconverted variable)
 */
int print(char* args[]){
	int i = 0;
	int flag = 0;
	while(args[i] != NULL){
		if (*args[i] == '$'){
			flag = 1;
		}
		else if(strcmp(args[i], "SUCCESS_CONVERT") != 0 ){
			printf("%s ", args[i]);
		}
		i++;
	}
	return flag;
}


void changeColour(char* colour, char* extraArg){
	if(colour == NULL){
		fprintf(stderr, "theme: missing colour\n");
		return;
	}
	if(extraArg != NULL){
		fprintf(stderr, "theme: too many arguments\n" );
		return;
	}

	char* rawInput = strdup(colour);
	// convert to uppercase
	for (int i=0; i< strlen(colour); i++){
		*(colour+i) = toupper(*(colour+i));
	}

	if(strcmp(colour, "RED") ==0 )
		printf("\033[31m");

	else if (strcmp(colour, "BLUE") ==0)
		printf("\033[34m");

	else if (strcmp(colour, "GREEN") ==0)
		printf("\033[32m");

	else if (strcmp(colour, "YELLOW") ==0)
		printf("\033[33m");

	else if (strcmp(colour, "MAGENTA") ==0)
		printf("\033[35m");

	else if (strcmp(colour, "CYAN") ==0)
		printf("\033[36m");

	else if (strcmp(colour, "RESET") ==0)
		printf("\033[37m");

	else{
		fprintf(stderr, "unsupported colour: %s\n", rawInput);

	}
	free(rawInput);

	return;
}

void leave(ELinkedList* envList, LinkedList* log, FILE* fp, char* input){
	printf("C you later!\n");
	deleteLog(log);
	deleteEnvList(envList);
	free(input);
	if(fp != stdin)
		fclose(fp);
	exit(0);

}
