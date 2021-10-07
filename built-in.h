#ifndef BUILT_IN_H
#define BUILT_IN_H


#define BUFFER_SIZE 256
#include "envVar.h"
#include "log.h"

void changeColour(char* colour, char* extraArgs);

int print(char* args[]);

char* formatInput(char* input, char* args[] );

void leave(ELinkedList* envList, LinkedList* log, FILE* fp, char* input);

#endif