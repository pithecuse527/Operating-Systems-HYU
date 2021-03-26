#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>

#define	READ_END	0
#define	WRITE_END	1
int value = 5;
int main(){

	pid_t pid;
  int fdA[2];

  if (pipe(fdA) == -1) {
		printf("PIPE ERROR\n");
		fprintf(stderr, "PipeA failed");
		return 1;
	}

	pid = fork();

	if (pid == 0) {  // child
		value += 15;
    close(fdA[READ_END]);
    write(fdA[WRITE_END], &value, sizeof(value));
    close(fdA[WRITE_END]);
		return 0;
	} else if (pid > 0) {  // parent
		wait(NULL);
    close(fdA[WRITE_END]);
    read(fdA[READ_END], &value, sizeof(value));
		printf("PARENT: value = %d\n", value); /* LINE A */
		return 0;
	}
}
