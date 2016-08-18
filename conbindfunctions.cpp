#include <unistd.h>
#include <stdio.h>
#include <string>
#include <errno.h>
#include <sys/wait.h>
using namespace std;
void repear( int killid){
	int status;
	printf("clean zoombie \n");
	while(waitpid(0,&status, WNOHANG)>0);
}

int createProcess(char * filename){
	char buffer[1024];
	sprintf (buffer, "./%s",filename);
	//printf("gogogo, %s",buffer);
	//
	int child_pid;
	if((child_pid=fork())==1){
		//fial Fork
		printf("I fall my people\n");
		_exit(1);
	}else if(child_pid==0) {
		execlp(buffer,filename,NULL);
	//	printf("gogogo, (%s)\n",buffer);
	//	printf("I am a child with ,pid : %d \n",getpid());
		_exit(1);
	}else{
	//	printf("I am a parent with ,pid : %d \n",getpid());
	}
}


int main(){
	int child_pid;
	char * line =NULL;
	size_t len =0;
	ssize_t read;
	
	signal(SIGCHLD,repear);

	printf("#");
	while((read=getline(&line,&len,stdin))!=-1){
		line[read-1]='\0';
		createProcess(line);
		wait(&child_pid);	
		printf("#");
	}
        return 0;
}


