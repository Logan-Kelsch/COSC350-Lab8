#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#define MAXLINE 256
void err_sys(char *str){
	printf("%s\n",str);
	exit(1);
}

int main(int argc, char *argv[]){
	if(argc!=3)
		err_sys("COMMAND LINE ERROR.\n");
	//bring and set ints from command line
	int newInts[2];
	int iter = 0;

	int n, fd[2];
	pid_t pid;
	char line[MAXLINE];
	if (pipe(fd) < 0) /* creat a pipe */
		err_sys("pipe error");
	if ( (pid = fork()) < 0) /*creat a child */
		err_sys("fork error");

	else if (pid > 0){ //parent
		close(fd[0]);
		sleep (3);

		write(fd[1], argv[1], sizeof(int));
		write(fd[1], argv[2], sizeof(int));
	}
	else{//child
		close(fd[1]);
		while(1){
			n = read(fd[0], line, MAXLINE);
			if(line==EOF)
				break;
			newInts[iter] = atoi(line);
		}
		int sum = newInts[0] + newInts[1];
		write(STDOUT_FILENO, "Sum of ints: ", 13);
		write(STDOUT_FILENO, sum, 2);
	}
	exit(0);
}
