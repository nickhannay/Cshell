#ifndef E_VARIABLE_H
#define E_VARIABLE_H

typedef struct Enode{
    struct Enode* next;
    struct EnvVar* data;
}Enode;

typedef struct ELinkedList{
    Enode* head;
    Enode* tail;
}ELinkedList;

typedef struct EnvVar{
	char *name;
	char *value;
}EnvVar;

ELinkedList* createEnvList();

void deleteEnvList(ELinkedList* list);

void addEnvVar(EnvVar* data, ELinkedList* list);

Enode* searchEnv(char* var , ELinkedList* list);

char* envVarConvert(char* args[], ELinkedList* envList);

EnvVar* createEnvVar(char* name, char* value );

#endif  