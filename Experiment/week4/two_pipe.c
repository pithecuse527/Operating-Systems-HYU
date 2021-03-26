#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define	BUFFER_SIZE	25
#define	READ_END	0
#define	WRITE_END	1

int main(void) {
	char write_msg[BUFFER_SIZE] = "Greetings";
	char read_msg[BUFFER_SIZE];
	int fdA[2];
  int fdB[2];
  int to_child = 0;
  int to_parent = 1000;
	pid_t pid;

	if (pipe(fdA) == -1) {
		printf("PIPE ERROR\n");
		fprintf(stderr, "PipeA failed");
		return 1;
	}

  if (pipe(fdB) == -1) {
		printf("PIPE ERROR\n");
		fprintf(stderr, "PipeB failed");
		return 1;
	}

	pid = fork();

	if (pid < 0) {
		printf("FORK ERROR\n");
		fprintf(stderr, "Fork failed");
		return 1;
	}

	if (pid > 0) { // parent
    close(fdA[READ_END]);
    close(fdB[WRITE_END]);
    while(to_parent < 1004){
  		write(fdA[WRITE_END], &to_child, sizeof(to_child)); // pass the message through pipe
      read(fdB[READ_END], &to_parent, sizeof(to_parent)); // read the message from child
      printf("parent got message: child %d\n", to_parent);
      sleep(1);
      to_child++;
    }
    wait(NULL); // wait for child before the parent terminate itself
	} else { // child
    close(fdB[READ_END]);
    close(fdA[WRITE_END]);
    while(to_child < 4){
  		write(fdB[WRITE_END], &to_parent, sizeof(to_parent)); // pass the message through pipe
      read(fdA[READ_END], &to_child, sizeof(to_child)); // read the message from parent
      printf("\tchild got message: parent %d\n", to_child);
      sleep(1);
      to_parent++;
    }
	}

	return 0;
}
