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
				pout() <<"Hello "<<getlogin()<<"!"<<endl;
				continue;
			}
			if((command)=="quit"){
				while(true){
					pout() <<"Are you sure you want to quit "<<getlogin()<<"? (Yes/No)"<<endl;
					getline(cin,command);

					if(command=="Yes"){
						command = "exit";
						break;
					}		
					else if(command == "No")
						break;
					else
						;
				}
			}

				
		}

	}

	exit(0);
	return 0;	
}
