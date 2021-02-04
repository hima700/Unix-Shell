#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>



int divideLine(char* args[], char line[])
{
    int i = 0;

    gets(line);
    line[strlen(line)] = '\0';



    char* pointer = strtok(line," ");

    while(pointer != NULL)
    {
        args[i] = pointer;
        pointer = strtok(NULL," ");
        i++;
    }
    return i;
}
int cd(char* path)
{
    return chdir(path);
}

void Handler(int signal)
{
    FILE* f;
    wait(NULL);
    f = fopen("x.txt","a");
    printf("\nChild process was terminated w id = %d \n",getpid());
    fprintf(f,"\nChild process was terminated w id = %d \n",getpid());
    fclose(f);
}
int main()
{
    char* args[10];
    char line[100];
    int c, backgroundProcess = 0;

    signal(SIGCHLD,Handler);

    while(1)
    {

        c = divideLine(args,line);
        pid_t childPID = fork();


        if(strcmp(args[0],"exit")==0)
            exit(0);

        if(strcmp(args[c-1],"&") == 0)
            backgroundProcess = 1;

        if(strcmp(args[0],"cd") == 0)
        {
            if(cd(args[1])<0)
                perror(args[1]);
            continue;   /* skip fork*/
        }

        if(childPID >= 0)  // fork success
        {
            if(childPID == 0)  //child proccess
            {
                if(execvp(args[0],args)<0)
                    perror(args[0]);
            }
            else    //parent process
            {
                if(backgroundProcess == 1)
                    continue;
                waitpid(childPID,NULL,0);  // wait until child process finishes
            }
        }
        else
            printf("Fork Failed\n");
    }
    return 0;
}
