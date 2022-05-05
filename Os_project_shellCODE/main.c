#include <string.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // execvp()
#include <sys/types.h>
#define COLOR_BLUE    "\x1b[34m"
#define COLOR_GREEN   "\x1b[32m"
#define COLOR_RED     "\x1b[31m"
#define COLOR_RESET   "\x1b[0m"

void terminateLine(char line[]);
void executeProcess(char *argv[], int flag);
int parseLine(char line[], char*tokens[]);
void handler(int sig);
void cd(const char* path);

int main()
{
    int background ;
    char *buffer = NULL;
    size_t n = 0;
    char *tokens[20];
    printf(COLOR_GREEN"*****This shell is prepared by IDs: 4,5,28,126 *****\n");
    while (1) {                   /* repeat until done ....         */
            for(int j=0;j<20;j++)
              tokens[j]=NULL;
        printf(COLOR_BLUE "Shell > "COLOR_RESET );     /*   display a prompt     */
        getline(&buffer,&n,stdin);            /*   read in the command line     */
        if (strcmp(buffer, "\n") == 0) { /* is it no command ?   */
               printf(COLOR_RED  "No Command\n"COLOR_RESET );
                  continue;  }
        terminateLine(buffer);
        printf("\n");
        background = parseLine(buffer,tokens);       /*   parse the line       */
         if (strcmp(tokens[0], "exit") == 0) { /* is it an "exit"?     */
               printf(COLOR_RED  "END of program"COLOR_RESET );
                  exit(0);  } /*   exit if it is      */
        executeProcess(tokens,background);          /* otherwise, execute the command */
     }
    return 0;
}

void handler(int sig)
{
  FILE *pointer;
  pointer=fopen("logfile.txt","a+");
  fprintf(pointer,"child is terminated \n");
  fclose(pointer);
}

void terminateLine(char line[]){
    int i=0;
    while(line[i] != '\n')
        i++;
    line[i] = '\0';
}

int parseLine(char line[], char*tokens[]){
   int i=0, flag=0;
   char *token;
   token = strtok(line, " "); /* get the first token */
   /* walk through other tokens */

   while( token != NULL ) {
      tokens[i] = token;
      i++;
      token = strtok(NULL," ");
   }
/*Detect background process*/
   if(strcmp(tokens[i-1], "&") == 0){
        flag = 1;
        tokens[i-1] = '\0';
   }
   return flag;
}

void  executeProcess(char *argv[], int flag)
{
     pid_t  pid;
     int    status;
     signal(SIGCHLD, handler);
     if ((pid = fork()) < 0) {     /* fork a child process           */
          printf(COLOR_RED  "* ERROR: forking child process failed\n");
          exit(1);
     }
     else if (pid == 0) {          /* for the child process:         */
          if(strcmp(argv[0], "cd") == 0){
               cd(argv[1]);
            }

          else if (execvp(argv[0], argv) < 0) {     /* execute the command  */
               printf(COLOR_RED  "* ERROR: exec failed\n");
               exit(1);
          }
     }
     else {                                  /* for the parent:      */
         if (flag == 0)
            while (wait(&status) != pid);       /* wait for completion  */
     }
}
/*Change directory*/
void cd(const char* path)
{
    char s[100];
    if (chdir(path)==0)
        printf("%s\n",getcwd(s,100));
    else
    printf(COLOR_RED  "change directory failed\n");
  exit(0);
}
