#include <stdlib.h>
#include <stdio.h>
using namespace std;
int main(){
	char * line =NULL;
	size_t len =0;
	getline(&line,&len,stdin);
	printf("%s",line);
        return 0;
}

