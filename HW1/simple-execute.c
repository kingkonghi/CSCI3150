#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

int shell_execute(char ** args, int argc)
{
	int child_pid, wait_return, status;


	pid_t pid;

	if ( strcmp(args[0], "EXIT") == 0 )
		return -1; 

	int i,j,k,m;
	unsigned int total=1;
	unsigned int temp = 1;


	//To calculate how many processes
	for(i = 0; i < argc-1; i++) {
		if(strcmp(args[i],"|")==0){
		total++;
		}
	}

	char** process[total];
	process[0]= &args[0];
	for(i=0;i< argc-1;i++){
		
		if(strcmp(args[i],"|")==0){
			args[i]= NULL;
			process[temp]= &args[i+1];
			temp++;
		}	
	}

	/*for(i=0;i<total;i++){
	printf("Process: %s \n",process[i][0]);
	}
	printf("Total process: %d\n",total);*/

	//Calculate total end

	int fo,foa;
	int fd[100][2];
	//if there is only one process. Eg: ls -1
	if(total==1){
	foa=fork();
	//printf("Fork 1:%d\n",foa);

		if(foa == 0){
			execvp(process[0][0], process[0]);}
		else{
			pid = wait(&status);
		return 0;
		}	
	}
	//if one end
	//if more than one process. And if child process
	fo = fork();
	//printf("Fork 1:%d\n",fo);

	if(fo==0){
		for(i=0;i<total - 1;i++){
			pipe(fd[i]);
			foa = fork();
			//printf("Fork %d:%d\n",i,foa);
			if(foa==0){
				if(i==0){
				//handle first process
					close(1);
					dup(fd[i][1]);
				}
				else{
				//handle remain process
					close(0);
					dup(fd[i-1][0]);

					close(1);
					dup(fd[i][1]);
				}
			for(j=0;j<=i;j++){
				close(fd[j][0]);
				close(fd[j][1]);
			}
			//Execute command and also handle error
			status = execvp(process[i][0],process[i]);
			if(status<0){
				printf("execvp() error \n");
				exit(-1);
			}
		}
	}
	
	close(0);
	dup(fd[total-2][0]);

	//close all pipefd
	for(j=0;j<total-1;j++){
		close(fd[j][0]);
		close(fd[j][1]);	
	}

	//Execute the last process and to handle Error
	status = execvp(process[total - 1][0],process[total - 1]);
	if(status<0){
		printf("execvp() error \n");
		exit(-1);
		}
	}
	//if more than one end
	else{
		if((pid = wait(&status))<0){
			printf("wait() error \n");
		}
	}
	usleep(50000);
	return 0;

}
