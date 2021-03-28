/*
FILENAME : ji_shell.c

DESCRIPTION :
  Designing a C program accepting user commands and serving in sort of shell's way

NOTES :
  This C file is designed for assignment1 given by Dr. Hee Kuck Oh

AUTHOR :  Hong Geun Ji      START DATE : 23 Mar 2021

CHANGES :
  NO   VERSION   DATE         WHO           DETAIL
  1    1.1       24 Mar 2021  Hong Geun Ji  Change the way to print the prompt string
  2    1.2       25 Mar 2021  Hong Geun Ji  Complete the command parsing
  3    1.3       26 Mar 2021  Hong Geun Ji  Separate the parsing function
  4    1.4       26 Mar 2021  Hong Geun Ji  Add forking with the special symbols such as '|', '>'
  5    1.5       27 Mar 2021  Hong Geun Ji  Error handling with exec functions
  6    1.6       28 Mar 2021  Hong Geun Ji  Remove the unused vars and revise the repetitive codes
  7    1.7       28 Mar 2021  Hong Geun Ji  Revise and add some comments
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>
#define SHELL_PROMPT "JI_SH> "  // default shell prompt
#define MAX_LINE 100 // the maximum length command
#define TRUE 1
#define FALSE 0
#define WRITE_END    1
#define READ_END    0
#define NORMAL_MODE 0 // execution modes
#define REDIRECTION_MODE_IN 1
#define REDIRECTION_MODE_OUT 2
#define PIPE_MODE 3

// the length of the prompt string
// need to change this as well if the prompt string is changed
#define PROMPT_BUFF_SIZE 7

 // parses the command and saves some info. (this returns the args location of ‘>’, ‘<’, and ‘|’)
int MyParse(char* usr_cmd, char** args, int* background, int* mode) ;

// fork by mode
int Execution(char** args, int mode, int background, int symbol_loc);

int main(void)
{
      char** args = (char**)malloc(MAX_LINE*sizeof(char*)/2+sizeof(char*));  // cmd line arguments
      char* usr_cmd = (char*)malloc(MAX_LINE*sizeof(char)+sizeof(char));     // user command line ready
      char*  prompt = SHELL_PROMPT; // string for the prompt
      int shell_is_alive;  // shell status
      int mode;         // execution mode when fork
      ssize_t cmd_len;    // the number of chars from user command input
      int special_symbol_loc; // the symbol location such as '|' or '>'
      int background;

      // prompt runs until it is killed by something or someone
      shell_is_alive = TRUE;
      while(shell_is_alive)
      {
            /**************** user command input area start *****************/
            // use system call to print right away
            if(!write(STDOUT_FILENO, prompt, PROMPT_BUFF_SIZE))
            {
                  // use stderr to let fprintf prints error right away
                  fprintf(stderr, "prompt error\n\n");
                  exit(EXIT_FAILURE);
            }
            memset(usr_cmd, '\0', MAX_LINE);  // reset the command line buffer

            if((cmd_len = read(STDIN_FILENO, usr_cmd, MAX_LINE+1)) > MAX_LINE)
            {
                  // use stderr file to let fprintf prints error right away
                  fprintf(stderr, "command error! may be too long?\n\n");
                  exit(EXIT_FAILURE);
            }
            if(*usr_cmd == '\n') continue;  // do nothing with the single newline
            *(usr_cmd+cmd_len-1) = '\0';    // swap the '\n' to null character
            if(strcmp(usr_cmd, "exit\0") == 0) break; // exit condition
            /**************** user command input area end *****************/

            special_symbol_loc = MyParse(usr_cmd, args, &background, &mode);
            if(Execution(args, mode, background, special_symbol_loc) == 0)
                 continue; // fork followed by mode
            else // execution failed
            {
                  printf("something went wrong...\n");
                  shell_is_alive = FALSE; // kill this shell process on execution error
            }
      } // while

      return 0;
}

int Execution(char** args, int mode, int background, int symbol_loc)
{
      pid_t pid;  // process id for non PIPE_MODE
      pid_t pid2; // process id for PIPE_MODE
      int status;

      pid = fork();
      if(pid < 0) // error
      {
            fprintf(stderr, "Fork failed");
            return 1;
      }
      else if(pid == 0) // child
      {
            int fd;
            int fdl[2];

            // use redirection according to the mode
            if(mode == REDIRECTION_MODE_OUT)
            {
                  printf("%s\n",*(args+symbol_loc+1));  // let the user knows the output file name
                  fflush(stdout);
                  fd = open(*(args+symbol_loc+1),
                              O_CREAT|O_RDWR|O_TRUNC,
                              0666);
                  dup2(fd, STDOUT_FILENO);
            } // end if REDIRECTION_MODE_OUT
            if(mode == REDIRECTION_MODE_IN)
            {
                  fd = open(*(args+symbol_loc+1),
                              O_CREAT|O_RDWR,
                              0666);
                  dup2(fd, STDIN_FILENO);
            } // end if REDIRECTION_MODE_IN

            // child use pipe on PIPE_MODE
            if(mode == PIPE_MODE)
            {
                  pipe(fdl);
                  pid2 = fork();  // create a child which will run the right part of the pipe symbol

                  if(pid2 < 0) // error
                  {
                        fprintf(stderr, "Fork2 failed");
                        return 1;
                  }
                  else if(pid2 == 0) // child again
                  {
                        dup2(fdl[READ_END], STDIN_FILENO);  // take the result from the parent
                        close(fdl[WRITE_END]);
                        execvp(*(args+symbol_loc+1), args+symbol_loc+1);  // execute the right part of the ‘|’
                        exit(1);  // exit on pipe error
                  }
                  else  // child as a parent
                  {
                        dup2(fdl[WRITE_END], STDOUT_FILENO);  // give the result to the child
                        close(fdl[READ_END]);
                        sleep(1); // give some time to ready for next command
                  }
            } // end if PIPEMODE

            execvp(*args, args);
            exit(1);  // exit on fail (such as meaningless command)
      } // else if
      else  // parent
      {
            if(background) {
                  fprintf(stdout, "[%d]\n", pid);
                  fflush(stdout);
            }
            else{      // wait only if it is a foreground
                  waitpid(pid, &status, 0);
            }
      } // else
      return 0;
}

int MyParse(char* usr_cmd, char** args, int* background, int* mode)
{

      int i = 0;
      int symbol_loc = 0;
      int args_len;

      // save the each token to args
      *args = strtok(usr_cmd, " ");
      while(*(args+i))
      {
            i++;
            *(args+i) = strtok(NULL, " ");  // parse the next location from right before
      }
      args_len = i; // save the number of args elements
      i = 0;
      *mode = NORMAL_MODE;
      *background = FALSE;

      // check whether the command should run on background or not
      if(strcmp(*(args+args_len-1), "&") == 0) {
            *background = TRUE;
            *(args+args_len-1) = NULL;
      }

      while(*(args+i))  // look up the every args until it finds what mode should be used
      {
            // printf("!%s!\n", *(args+i));  // testing what the tokens are
            if(strcmp(*(args+i), ">") == 0)  // found “>”
            {
                  *mode = REDIRECTION_MODE_OUT;
                  *(args+i) = NULL;
                  symbol_loc = i;
                  break;
            }
            if(strcmp(*(args+i), "<") == 0) // found “<”
            {
                  *mode = REDIRECTION_MODE_IN;
                  *(args+i) = NULL;
                  symbol_loc = i;
                  break;
            }
            if(strcmp(*(args+i), "|") == 0) // found “|”
            {
                  *mode = PIPE_MODE;
                  *(args+i) = NULL;
                  symbol_loc = i;
                  break;
            }
            i++;
      } // while
      return symbol_loc;
}
