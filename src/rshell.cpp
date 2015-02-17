// Chuanping "Ping" Fan
// 2015 cfan002@ucr.edu
//
//

//test macro

#include <iostream>
#include <cstring>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <vector>
#include <fstream>
#include <sys/wait.h>
#include <algorithm>
#include <boost/algorithm/string/replace.hpp>

using namespace std;

//display command prompt symbol before every output
ostream& pout(){
	char hostname[1024];
	
	hostname[1023] = '\0';
	gethostname(hostname,1023);

	return cout <<getlogin()<<"@"<<hostname<< "$ ";
}

vector<char*> parsestring(char* x){
	
	vector<char*> parsed;
	char* delim;

	delim = strtok(x," ");
	parsed.push_back(delim);
		
	while(delim != NULL){

		delim = strtok(NULL, " " );
		parsed.push_back(delim);
		
	}

	return parsed;
}

//split up string into something useable
string filterstr(string userin){
	//first remove comments
	string newstr = userin.substr(0, userin.find('#'));
	

	//checks beginning and end of string for hanging characters
	if(!newstr.empty()){
		while(newstr[0]==' '){
				newstr.erase(0,1);	
			}

		if(newstr[0]==';' || newstr[0]=='|' || newstr[0]=='&'){
			pout()<<"\'"<<newstr.at(0)<<"\' "<< "cannot be at the start of your command."<<endl;
			return "";
		}
	}
	
	//get rid of empty spaces at the end of the string for formatting
	while(!newstr.empty()){
		if(newstr.at(newstr.size()-1)==' '){
			newstr.resize(newstr.size()-1);	
		}
		else break;
	}
	
	//check for special char at end
	if(newstr.at(newstr.size()-1)==';' || newstr.at(newstr.size()-1)=='|' || newstr.at(newstr.size()-1)=='&'){
		pout()<<"\'"<<newstr.at(newstr.size()-1)<<"\' "<< "cannot be at the end of your command."<<endl;
		return "";
	}
	
	//formatting so that it will be easier to parse.
	boost::replace_all(newstr, "||", " || ");
	boost::replace_all(newstr, "&&", " && ");
	boost::replace_all(newstr, ";", " ; ");

	return newstr;
}

bool execute(vector<char*> cmdlist){
	int pid = fork();
	int sz = cmdlist.size();
	int stat = 0;
	char **cmds = new char*[sz+1];

	for(int i=0; i<sz; i++){
		cmds[i] = cmdlist[i];
	}

	cmds[sz] = '\0';

	if(pid<0){
		perror("Forking Error");
	}

	else if(pid==0){
		//child process
		if(execvp(cmds[0], cmds)==-1)
			perror("execvp error");
		exit(1);
	}

	else{
		if(waitpid(pid,&stat,0)==-1){
			perror("error executing");
			exit(1);
		}	
	}

	delete [] cmds;

	if (stat==0)
		return true;
	else
		return false;
	
}

int main(int argc, char* argv[]){
	
	string command;
	char raw[1080];
	vector<char*> currcom;
	bool ifsucceed = true;

	while(command!="exit"){

		pout();
		getline(cin,command);

		command = filterstr(command);
		strcpy(raw, command.c_str());
		currcom = parsestring(raw);
		
		for (size_t i=0; i<currcom.size(); i++){
			pout()<<currcom.at(i)<<" "<<endl;	
		}
		
		//forloop through currcom
		//make sep function for || && take in currcomm as param
		//use global bool to keep track of last successful command?
			
		execute(currcom);
	}
	
	return 0;	

}
