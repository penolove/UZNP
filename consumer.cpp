#include <iostream>
#include <signal.h>
#include <string>
using namespace std;

void my_handler (int param)
{
  	cout<<"EWRWERWER\n";
	
}
int main(){
	string line;
	signal (SIGUSR1, my_handler);

	while(getline(cin,line)){
		cout<<line<<", ~PU\n";
	}

        return 0;
}

