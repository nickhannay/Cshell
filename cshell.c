#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "built-in.h" // contains formatInput(), print(), leave() and changeColour()
#include "log.h"      // contains functions related to the logged commands
#include "envVar.h"   // contains functions related to environment variables

int main(int argc, char const *argv[]) {
	char*        command;
	char         output[BUFFER_SIZE];
	char*        args[BUFFER_SIZE];
	int          numbytes;
	FILE*		 commandStream = stdin;
	ELinkedList* envList = createEnvList();
	LinkedList*  log = createLog();


	if (envList == NULL){
		fprintf(stderr, "Failed to allocate space for environment variable list\n");
		return(1);
	}
	if (log == NULL){
		fprintf(stderr, "Failed to allocate space for environment command log\n");
		return(1);
	}

	if(argc > 1){
		// script mode
		char file[BUFFER_SIZE];
		strcpy(file, (char *)argv[1]);

		commandStream = fopen(file, "r");
		while( commandStream == NULL){
			fprintf(stderr, "failed to open file: %s\nEnter file: ", file);

			fgets(file,BUFFER_SIZE,stdin);
			file[strcspn(file, "\n")] = '\0';

			commandStream = fopen(file, "r");
		}

	}
	// *MAIN LOOP* //
	while(true){
		char* rawInput = malloc(sizeof(char)* BUFFER_SIZE);

		// only print in interactive mode
		if(commandStream == stdin){
			printf("cshell$ ");
			fflush(stdout);
		}


	 	if(fgets(rawInput, BUFFER_SIZE , commandStream) == NULL){
	 		leave(envList, log, commandStream, rawInput);
	 	}

		command = formatInput(rawInput, args);

		// add new command to log with defualt error code 0
		// error code modified by parent after child exits
		Command* newCommand = createCommand(command);
		if( newCommand == NULL){
			fprintf(stderr, "failed to allocate space for new command\n" );
			return(1);
		}
		addCommand(newCommand, log);

		// replace each valid environment variable with its value and check for exit command
		if((command = envVarConvert(args, envList)) != NULL){
			if(strcmp(command, "exit") == 0){
				leave(envList, log, commandStream, rawInput);
			}
		}

		// open pipes to allow child to communicate with parent
		int stdoutPipe[2];
		int stderrPipe[2];
		if(pipe(stdoutPipe) == -1 || pipe(stderrPipe) == -1){
			perror("error opening pipe\n");
			return(1);
		};

		// create new process to handled input
		pid_t pid = fork();
		switch (pid) {
		    case -1:
		    	perror("fork");
				exit(1);
				break;

		    case 0:
		    // ** CHILD *** ///
		    	close(stdoutPipe[0]);
		    	close(stderrPipe[0]);
		    	if (dup2(stdoutPipe[1], 1) == -1 || dup2(stderrPipe[1], 2) == -1){
		    		perror("Pipe");
		    		exit(1);
		    	};
		    	close(stdoutPipe[1]);
				close(stderrPipe[1]);

				if(command == NULL){
					fprintf(stderr, "\n");
				}
		      	else if(strcmp("print", command) == 0){
		      		// parent sets log code depending on which pipe the child writes too
		      		// stderrPipe = log code : -1  - stdoutPipe = log code: 0
					if(print(args+1) == 1)
						fprintf(stderr, "\n");
					else
						fprintf(stdout, "\n");
				}
				else if (strcmp("log", command) == 0){
					if(args[1] == NULL)
						printLog(log);
					else
						fprintf(stderr, "log: to many arguments\n");
				}
				else if (strcmp("theme", command) == 0){
					changeColour(args[1], args[2]);
				}
				else if(strcmp("SUCCESS_CONVERT", command) == 0 ){
					//environment variable set successfully - do nothing
				}
				else{
					execvp(args[0], args);

					// falls through when command unrecognized
					fprintf(stderr, "%s: No such file or command\n" , command);

				}
				fflush(stdout);
				fflush(stderr);
				_exit(0);

		    default:
		    // **PARENT** //
		    	close(stdoutPipe[1]);
		    	close(stderrPipe[1]);

		    	// reading from stdout pipe
		    	while ((numbytes = read(stdoutPipe[0], &output, BUFFER_SIZE-1)) > 0 ){
		    		output[numbytes] = '\0';
		    		printf("%s",output);
		    	}
		    	// reading from stderr pipe
		    	if ((numbytes = read(stderrPipe[0], &output, BUFFER_SIZE-1)) > 0){
		    			output[numbytes] = '\0';
			    		printf("%s",output);
			    		log->tail->data->code = -1;

			    		while ((numbytes = read( stderrPipe[0], &output, BUFFER_SIZE-1) ) >0){
				    		output[numbytes] = '\0';
				    		printf("%s",output);
			    		}
		    	}
				wait(NULL);

				close(stdoutPipe[0]);
				close(stderrPipe[0]);
				break;
	    }
		free(rawInput);
	}

  return 0;
}
