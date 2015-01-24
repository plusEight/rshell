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
	//not sure if I need anymore
}

int main(int argc, char* argv[]){
	
	string command;
//moved char declaration
//because declaration might give me trouble? not sure if i can declare empty array of char
	while(command!="exit"){

		pout();
		getline(cin,command);
// basic commands for simplicity

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
//now we parse and do real stuff
//ok, convert vector to char array
	
		else{

			int pid = fork();
			vector<string> parsed = parsein(command);

			char* scommand[parsed.size()]; 
			const char* firstexec;

			firstexec = parsed.at(0).c_str();

			for (int i=1;i<parsed.size();i++){
				scommand[i] = (char*)parsed.at(i).c_str();
			}

			if(pid==-1){
				perror("fork error");
				exit(1);
			}
			else if(pid==0){
				//child process
				execvp(firstexec,scommand);
				exit(1);
			}

			else{
				if(-1==wait(0))
					perror("parent process");	
			}

		}


	}

	exit(0);
	return 0;	


}
