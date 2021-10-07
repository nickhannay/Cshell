all: cshell

cshell: cshell.o built-in.o log.o envVar.o
	gcc -g -W cshell.o built-in.o log.o envVar.o -o cshell

cshell.o: cshell.c
	gcc -c cshell.c

built-in.o: built-in.c 
	gcc -c built-in.c

log.o: log.c
	gcc -c log.c

envVar.o: envVar.c
	gcc -c envVar.c

clean:
	rm *.o cshell 
