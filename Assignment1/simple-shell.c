/*
FILENAME : simple-shell.c

DESCRIPTION :
  Designing a C program accepting user commands and serving in sort of shell's way

NOTES :
  This C file is designed for assignment1 given by Dr. Hee Kuck Oh

AUTHOR :  Hong Geun Ji      START DATE : 23 Mar 2021

CHANGES :
  NO   VERSION   DATE         WHO           DETAIL
  1    1.1       24 Mar 2021  Hong Geun Ji  Change the way to print the prompt string
  2    1.2       25 Mar 2021  Hong Geun Ji  Complete the command parsing
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>

#define MAX_LINE 20 // the maximum length command
#define TRUE 1
#define FALSE 0
#define SHELL_PROMPT "osh> "

#define NORMAL_MODE 0 // execution modes
#define REDIRECTION_MODE_IN 1
#define REDIRECTION_MODE_OUT 2
#define PIPE_MODE 3

// the length of the prompt string
// need to change this as well if the prompt string is changed
#define PROMPT_BUFF_SIZE 6

int Execution(char** args, int n, int mode);  // fork by mode

int main(void)
{
  char** args = (char**)malloc(MAX_LINE*sizeof(char*)/2+sizeof(char*)); // cmd line arguments
  char* usr_cmd = (char*)malloc(MAX_LINE*sizeof(char)+sizeof(char));    // user command line ready
  char* prompt = SHELL_PROMPT; // string for the prompt
  int shell_is_alive; // shell status
  int mode;        // execution mode when fork
  int i;          // index for the args
  int cmd_len;    // the number of chars from user command input
  int args_len;   // the number of char* elements from args

  // prompt runs until it meets the end moment
  shell_is_alive = TRUE;
  while(shell_is_alive)
  {
    /**************** user command input area *****************/
    // use system call to print right away
    if(!write(STDOUT_FILENO, prompt, PROMPT_BUFF_SIZE))
    {
      // use *stderr to let fprintf prints error right away
      fprintf(stderr, "prompt error\n\n");
      exit(EXIT_FAILURE);
    }
    if((cmd_len = read(STDIN_FILENO, usr_cmd, MAX_LINE+1)) > MAX_LINE)
    {
      // use stderr file to let fprintf prints error right away
      fprintf(stderr, "command error! may be too long?\n\n");
      exit(EXIT_FAILURE);
    }
    *(usr_cmd+cmd_len-1) = '\0'; // swap the '\n' to null character
    /**************** user command input area *****************/


    /**************** command parsing area *****************/
    i = 0;
    *args = strtok(usr_cmd, " ");
    while(*(args+i))
    {
      i++;
      *(args+i) = strtok(NULL, " ");  // parse the next location from right before
    }
    args_len = i; // save the number of agrs elements
    /**************** command parsing area *****************/

    /**************** fork area *****************/
    i = 0;
    mode = NORMAL_MODE;
    while(*(args+i))  // choose mode from args
    {
      if(strcmp(*(args+i), ">") == 0)  // we found there is a file redirection (output)
      {
        mode = REDIRECTION_MODE_OUT;
        strcpy("", *(args+i));
      }
      else if(strcmp(*(args+i), "<") == 0) // we found there is a file redirection (input)
      {
        mode = REDIRECTION_MODE_IN;
        strcpy("", *(args+i));
      }
      Execution(args, args_len, mode);
      i++;
    }
    /**************** fork area *****************/

  } // while

  return 0;
  // todo: test git push
}

int Execution(char** args, int n, int mode)
{
  int fd;
  int fd_changed = FALSE;
  if(mode == REDIRECTION_MODE_OUT)
  {
      fd = open(*(args+n-1), O_CREAT|O_RDWR|O_TRUNC);
      dup2(fd, STDOUT_FILENO);
      fd_changed = TRUE;
  }
  if(mode == REDIRECTION_MODE_IN)
  {
      fd = open(*(args+n-1), O_CREAT|O_RDWR);
      dup2(fd, STDIN_FILENO);
      fd_changed = TRUE;
  }

  pid_t pid = fork();
  if(pid < 0) // error
  {
    fprintf(stderr, "Fork failed");
    return 1;
  }
  else if(pid == 0) // child
  {
    execvp(*args, args);
    printf("execvp call was unsuccessful\n");
  }
  else  // parent
  {
    waitpid(pid, NULL, 0);
  }
  return 0;
}
