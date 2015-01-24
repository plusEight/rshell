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
#include <vector>
#include <fstream>
#include <sys/wait.h>

using namespace std;

//display command prompt symbol before every output
ostream& pout(){
	char hostname[1024];
	
	hostname[1023] = '\0';
	gethostname(hostname,1023);

	return cout <<getlogin()<<"@"<<hostname<< "$ ";
}

//check if string is the name of a file in directory

bool ifexist(string in){
	const char* sstring;
	sstring = in.c_str();

	fstream test(sstring);

	if(test.good()){
		test.close();
		return true;
	}

	else{
		test.close();
		return false;
	}
}
//split up string into something useable
vector<string> parsein(string userin){
	vector<string> rpar;	
	char* par;
	char convstr[256];
	strcpy(convstr,userin.c_str()); 

	par = strtok (convstr," |&;");
	while(par!= NULL){
		rpar.push_back(par);
		par = strtok(NULL," |&;");
			
	}
	return rpar;
}

//execs a file
void runfile(string in, char* list[]){
	const char* sstring;
	sstring = in.c_str();
	

	int pid = fork();

	if(pid==-1){
		perror("fork error");
		exit(1);
	}
	else if(pid==0){
		//child process
		execvp(sstring,NULL);
		exit(1);
	}

	else{
		if(-1==wait(0))
			perror("parent process");	
	}
}

int main(int argc, char* argv[]){
	
	string command;
	char* arglist[10];
	char* currargs[10];


	while(command!="exit"){

		pout();
		getline(cin,command);
	//parse here
		


		if(command.empty()){
			continue;
		}
			
		if((command)=="hello"){
			pout() <<"Hello "<<getlogin()<<"!"<<endl;
			continue;
		}
		else if((command)=="quit"){
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

		else if(command=="test"){
			vector<string> j;

			j = parsein("Hello myname is&&ping");

			for(int i=0; i<j.size(); i++){
				pout()<<j.at(i)<<endl;
			}
		}
		else if(ifexist(command)==true){
//			cout << "file exists!" << endl;
//			ok this works
			runfile(command, NULL);


		}

		else{
			pout() << "Invalid command. Please enter another command."<<endl;
		}

				
	}

	exit(0);
	return 0;	
}
