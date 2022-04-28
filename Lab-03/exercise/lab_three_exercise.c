
/*your code here*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <sys/types.h> 
#include <fcntl.h>

int main(void){
	int fo1,fo2;
	int pipefd[3][2];
	int rv1,rv2;
	char *const ls_argv[] = {"ls" , "-l" , NULL};
	char *const grep_argv[] = {"grep","D",NULL};
	char *const wc_argv[] = {"wc","-c",NULL};

	rv1 = pipe(pipefd[0]);
	assert(rv1 > -1);
	rv2 = pipe(pipefd[1]);
	assert(rv2 > -1);
	fo1 = fork();
	assert(fo1 > -1);
	

	if(fo1>0){
		fo2 = fork();
		assert(fo2 > -1);
		
		if(fo2>0){
			close(1);
			dup(pipefd[0][1]);
			close(pipefd[0][0]);
			close(pipefd[0][1]);
			execvp("ls",ls_argv);
		}else{
			close(0);
			dup(pipefd[1][0]);
			close(pipefd[0][0]);
			close(pipefd[0][1]);
			close(pipefd[1][0]);
			close(pipefd[1][1]);
			execvp("wc",wc_argv);	
		}
	}else{
		close(0);
		dup(pipefd[0][0]);
		close(1);
		dup(pipefd[1][1]);
		close(pipefd[0][0]);
		close(pipefd[0][1]);
		close(pipefd[1][0]);
		close(pipefd[1][1]);
		execvp("grep",grep_argv);
		
	}

}