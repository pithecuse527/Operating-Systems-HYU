#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>

int main()
{
  pid_t pid;
  pid = fork();
  if(pid < 0) // error
  {
    fprintf(stderr, "Fork failed");
    return 1;
  }
  else if(pid == 0) // child
  {
    printf("==============================\n");
    printf("CHILD: ls command\n");
    execlp("/bin/ls", "ls", NULL);
    printf("execlp call was unsuccessful\n");
  }
  else  // parent
  {
    wait(NULL);
    // waitpid(pid, NULL, 0);
    printf("\n---------------\n");
    printf("PARENT: Child Complete\n");
    printf("---------------\n");
  }
  return 0;
}
