#ifndef LOG_H
#define LOG_H

#include <time.h>

typedef struct Node{
    struct Node* next;
    struct Command* data;
}Node;

typedef struct LinkedList{
    Node* head;
    Node* tail;
}LinkedList;


typedef struct Command{
    char* name;
    char* time;
    int code;
}Command;


LinkedList* createLog();

void        deleteLog(LinkedList* log);

void        printLog(LinkedList* log);

Command*    createCommand(char* name );

void        addCommand(Command* data, LinkedList* list);


#endif





