#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "envVar.h"


void addEnvVar(EnvVar* var, ELinkedList* list){
	Enode* node = malloc(sizeof(Enode));
	if(node == NULL){
		fprintf(stderr, "failed to allocate space for new environment variable\n");
		exit(1);
	}
	
	node -> data = var;
	node -> next = NULL;

	if(list -> tail != NULL)
		list -> tail -> next = node;
	else
		list -> head = node;

	list -> tail = node;
	return;
}

/*
 *	Creates and returns an empty envVar
 *  or NULL on error
 */
EnvVar* createEnvVar(char* name , char* value ){
	EnvVar* newEnvVar = malloc(sizeof(EnvVar));
	newEnvVar -> name = strdup(name);
	newEnvVar-> value = strdup(value);
	return(newEnvVar);
}

/*
 * Creates and returns an empty Elinked list used to hold environment variables
 * or NULL on error
 */
ELinkedList* createEnvList(){
	ELinkedList* list =  malloc(sizeof(ELinkedList));
	list -> head = NULL;
	list -> tail = NULL;
	return(list);
}


void deleteEnvList(ELinkedList* list){
	Enode* trail = list->head;
	if(trail == NULL)
		return;

	Enode* ahead = list-> head ->next;
	while(ahead != NULL){
		free(trail -> data -> value);
		free(trail -> data -> name);
		free(trail -> data);
		free(trail);
		trail = ahead;
		ahead = ahead -> next;
	}
	free(trail);
	free(list);
	return;
}

/*
 * This function searches <list> for the Enode containing <var> and
 * returns the Enode containing <var> or NULL if not found
 */
Enode* searchEnv(char* var , ELinkedList* list){
	Enode* sNode = list -> head;
	while (sNode != NULL){
		if(strcmp(var, sNode-> data -> name) == 0){
			// environment variable found, return Enode in list
			return(sNode);
		}
		sNode = sNode -> next;
	}
	return(NULL);
}

/*
 * This function checks user input for environment variables of the form $<name>=<value> or $<name>.
 * argument: $<name>=<value> -> replaced with -> SUCCESS_CONVERT
 *           $<name>         -> replaced with -> <value>
 *
 * function returns:  NULL on error  or the arg[0] (command) on success
 */
char* envVarConvert(char* args[] , ELinkedList* envList){
	char* name;
	char* arg;
	char* val;
	int   length;
	int   errorFlag = 0;

	// check input for environment variables
	int i = 0;
	while ((arg = args[i]) != NULL){
		if(*arg == '$'){
			length = strlen(arg);
			name = strtok(arg, "=");
			
			// no equal sign -> convert <name> to <val>
			if (strlen(name) == length){

				// look for <name> in Envlist
				Enode* node = searchEnv(name+1, envList);
				if(node == NULL){
					fprintf(stderr, "%s: environment variable not found    ", name+1);
					errorFlag = 1;
				}
				else {
					// environment variable <name> found, replace with <value>
					args[i] = node -> data -> value;
				}
			}
			// equal sign present -> change existing variable or make new variable
			else{
				val = strtok(NULL, "=");

				// no <value>
				if(val == NULL){
					fprintf(stderr,"error setting environment variable - missing variable value");
					return NULL;
				}

				// no <name>
				if(strlen(name) == 1){

					fprintf(stderr, "error setting environment variable - missing variable name");
					return NULL;
				}

				// search for <name> in environment variable list
				Enode* node = searchEnv(name+1, envList);
				if(node == NULL){

					// envVar <name> not in list, add new envVar
					EnvVar* newVar = createEnvVar(name+1, val);
					addEnvVar(newVar, envList);
				}
				else {

					// envVar <name> found in list, change <value> of existing envVar
					node->data->value = strdup(val);
				}
				// cshell main ignores the command = SUCCESS_CONVERT
				// print() ignores arguments = SUCCESS_CONVERT
				// prevents cshell from displaying error message on successful envVar assignment
				args[i] = "SUCCESS_CONVERT";
			}
		}
		i++;
	}

	// flag used to allow child process to communicate error with parent
	// needed to accurately set log code of print() function upon error
	if (errorFlag == 1)
		return NULL;
	else
		return args[0];
}
