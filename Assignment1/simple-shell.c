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

#define MAX_LINE 20 // the maximum length command
#define TRUE 1
#define FALSE 0

// the length of the prompt string
// need to change this as well if the prompt string is changed
#define PROMPT_BUFF_SIZE 6

int main(void)
{
  char** args = (char**)malloc(MAX_LINE*sizeof(char*)/2+sizeof(char*)); // cmd line arguments
  char* usr_cmd = (char*)malloc(MAX_LINE*sizeof(char)+sizeof(char));   // user command line ready
  char* prompt = "osh> "; // string for the prompt
  char* tmp_str;  // tmp string for token
  int i = 0;  // index for the args
  int cmd_len; // the length of the user command line

  // prompt runs until it meets
  while(TRUE)
  {
    /**************** user command input sequence *****************/
    // use system call to print right away
    if(!write(STDOUT_FILENO, prompt, PROMPT_BUFF_SIZE))
    {
      // use *stderr to let fprintf prints error right away
      fprintf(stderr, "prompt error\n\n");
      exit(EXIT_FAILURE);
    }
    if((cmd_len = read(STDIN_FILENO, usr_cmd, MAX_LINE+1)) > MAX_LINE)
    {
      // use *stderr to let fprintf prints error right away
      fprintf(stderr, "command error! may be too long?\n\n");
      exit(EXIT_FAILURE);
    }
    /**************** user command input sequence *****************/

    *(usr_cmd+cmd_len-1) = '\0'; // swap the '\n' to null character

    /**************** parsing command *****************/
    *args = strtok(usr_cmd, " ");
    while(*(args+i))
    {
      i++;
      *(args+i) = strtok(NULL, " ");  // parse the next location from right before
    }
    /**************** parsing command *****************/

  } // while

  return 0;
  // todo: test git push
}
