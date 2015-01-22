// Chuanping "Ping" Fan
// 2015 cfan002@ucr.edu
//
//


#include <iostream>
#include <cstring>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

using namespace std;

//display command prompt symbol before every output
ostream& pout(){
	char hostname[1024];
	
	hostname[1023] = '\0';
	gethostname(hostname,1023);

	return cout <<getlogin()<<"@"<<hostname<< "$ ";
}

int main(int argc, char* argv[]){
	
	string command;
	while(command!="exit"){
	//parse commands
	
	


	//general stuff and commands
		pout();
		getline(cin,command);
		
		for(int i=0; i<argc; i++){
			
			if((command)=="hello"){
				pout() <<"this program works"<<endl;
				continue;
			}
				
		}

	}
	return 0;	
}
