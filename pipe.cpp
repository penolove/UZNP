
#include <unistd.h>
#include <stdio.h>
#include <string>
using namespace std;

int PipeProcess(char * filename,int pipeRe,int pipeOut){
	char buffer[1024];
	sprintf (buffer, "./%s",filename);
	int child_pid;
	if((child_pid=fork())==1){
		printf("I fall my people\n");
		_exit(1);
	}else if(child_pid==0) {
		dup2(pipeRe,STDOUT_FILENO);
		dup2(pipeOut,STDIN_FILENO);
		execlp(buffer,filename,NULL);
		_exit(1);
	}else{
	}
}


int main(){
	int pipefd[2];
	pipe(pipefd);	
	
	PipeProcess("consumer",STDOUT_FILENO,pipefd[0]);
	PipeProcess("producer",pipefd[1],STDIN_FILENO);
        return 0;
}


