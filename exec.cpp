#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
using namespace std;
int main(){
	FILE * pFile;
	pFile = fopen ("trial.txt","w");
	dup2( fileno(pFile), STDOUT_FILENO);
	execlp("./producer",NULL);
        return 0;
}

