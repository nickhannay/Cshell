#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "log.h"

/*
 * Creates and returns an empty LinkedList used to log commands
 * or NULL on error
 */
LinkedList* createLog(){
	LinkedList* list =  malloc(sizeof(LinkedList));
	list -> head = NULL;
	list -> tail = NULL;
	return(list);
}


void deleteLog(LinkedList* list){
	Node* trail = list->head;
	if(trail == NULL)
		return;

	Node* ahead = list-> head ->next;
	while(ahead != NULL){
		free(trail -> data -> time);
		free(trail -> data -> name);
		free(trail -> data);
		free(trail);
		trail = ahead;
		ahead = ahead -> next;
	}

	free(trail -> data -> time);
	free(trail -> data -> name);
	free(trail -> data);
	free(trail);
	free(list);

	return;
}

/*
 * Traverse the LinkedList <log> from start to finish, printing each command.
 * Doesn't print the last log command that invoked this function
 */
void printLog(LinkedList* log){
	Node* node = log -> head;

	while(node != log->tail){
		Command* command = node -> data;
		printf("%s %s %d\n",
		  command -> time,
		  command -> name,
		  command -> code);

		node = node -> next;
	}
	return;
}

/*
 * 	Adds the newest command <data> to the end of the log <list>
 */
void addCommand(Command* data, LinkedList* list){
	Node* node = malloc(sizeof(Node));
	if(node == NULL){
		fprintf(stderr, "failed to add command to log\n");
		exit(1);
	}
	node -> data = data;
	node -> next = NULL;

	if(list -> tail != NULL)
		list -> tail -> next = node;
	else
		list -> head = node;

	list -> tail = node;
	return;
}

/*
 * Creates and returns an empty command with default log code 0
 * or NULL on error
 */
Command* createCommand(char* name ){
	Command* newCommand = malloc(sizeof(Command));
	time_t now = time(NULL);

	newCommand -> name = strdup(name);
	newCommand -> code = 0;
	newCommand -> time = strdup(ctime(&now));


	return(newCommand);
}
