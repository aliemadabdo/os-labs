//project notes:
//-zombi process: when child terminates befor parent process
//-orphan process: when parent terminates befor child process
//
//

#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>


#define SIZE_OF_CMD 256 //256 Char
#define ARG_BUFFER 100 

void log_handle(int sig){
	//printf("[LOG] child proccess terminated.\n");
	FILE *pFile;
        pFile = fopen("log.txt", "a");
        if(pFile==NULL) perror("Error opening file.");
        else fprintf(pFile, "[LOG] child proccess terminated.\n");
        fclose(pFile);
}

int main(){
    char cmd[SIZE_OF_CMD];
    char arg[ARG_BUFFER];

    while(1){

        
        
        fgets(cmd, SIZE_OF_CMD, stdin); // (destination, size and standared input (keyboard))
        if ((strlen(cmd) > 0) && (cmd[strlen (cmd) - 1] == '\n')) //check for input
                cmd[strlen (cmd) - 1] = '\0'; // convert to C string "add null chararacter"

        //validate input
        // for (int i=0; i<sizeof(cmd);i++){
        //     if ( (!isalnum(cmd[i])) && cmd[i] != '-' && cmd[i] != '_'  && cmd[i] != ' ' ) {
        //         printf("command not found");
        //         break;
        //     }
        //     else {
        //         printf("%c",cmd[i]);
        //     }
        // }

        if      (strcmp("",cmd))     continue;         //no input case
        if (strcmp("exit",cmd)) break;            //exit the shell


        char *ptr = strtok(cmd, " "); 
        while(ptr != NULL){
            //printf("%s\n", ptr);
            argv[i] = ptr;
            i++;
            ptr = strtok(NULL, " ");
        }

            // check for "&"
        if(!strcmp("&", argv[i-1])){
        argv[i-1] = NULL;
        argv[i] = "&";
        }else{
        argv[i] = NULL;
        }
        //printf("%d\n", i);
    
    }
    printf("\n \n");
    return 0;
}