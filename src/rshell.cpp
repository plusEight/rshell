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

ostream& pout(){
	char hostname[1024];
	hostname[1023] = '\0';
	gethostname(hostname,1023);
	return cout <<getlogin()<<"@"<<hostname<< "$ ";
}

vector<char*> parsestring(const string x){
	vector<char*> parsed;
	char* delim;

	delim = strtok(const_cast<char*>(x.c_str())," ");
	parsed.push_back(delim);
		
	while(delim != NULL){
		delim = strtok(NULL, " " );
		parsed.push_back(delim);
	}
	
	parsed.pop_back(); //for some reason creates empty element?
	return parsed;
}

string filterstr(const string userin){
	string newstr = userin.substr(0, userin.find('#'));
	if(!newstr.empty()){
		while(newstr[0]==' '){
			newstr.erase(0,1);	
		}

		if(newstr[0]==';' || newstr[0]=='|' || newstr[0]=='&'){
			pout()<<"\'"<<newstr[0]<<"\' "<< "cannot be at the start of your command."<<endl;
			return "";
		}
		//out of range here
		if(newstr.empty())
			return newstr;

		while(1){
			if(newstr.at(newstr.size()-1)==' '){
				newstr.resize(newstr.size()-1);	
			}
			else break;
		}

		if(newstr.at(newstr.size()-1)==';' || newstr.at(newstr.size()-1)=='|' || newstr.at(newstr.size()-1)=='&'){
			pout()<<"\'"<<newstr.at(newstr.size()-1)<<"\' "<< "cannot be at the end of your command."<<endl;
			return "";
		}
	
	}

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
		return true;
	}
	else{
		if(waitpid(pid,&stat,0)==-1){
			perror("error executing");
			exit(1);
		}	
	}

	delete [] cmds;
	return false;
}

vector<char*> splitcommand(vector<char*> &x, int &y){
	vector<char*> lhs;
	size_t i=0;
	if (x.empty())
		return lhs;

	if((strcmp(x.at(i),";")==0)){
		x.erase(x.begin());
		y = 0;
	}
	if((strcmp(x.at(i),"&&")==0)){
		x.erase(x.begin());
		y = 1;
	}
	if((strcmp(x.at(i),"||")==0)){
		x.erase(x.begin());
		y = 2;
	}
	
	for (i=0; i<x.size(); i++){
		if((strcmp(x.at(i),"||")==0) || (strcmp(x.at(i),"&&")==0) || (strcmp(x.at(i),";")==0))
			break;
		lhs.push_back(x.at(i));
	}

	x.erase(x.begin(), x.begin()+i);

	return lhs;
}

void workcommand(const string userin){
	string input = filterstr(userin);
	vector<char*> cmdlist = parsestring(input);
	vector<char*> splitlist;
	int tracker = -1;
	bool prevcmd = true; //prev command succeeded or failed
	bool firstrun = true;
	
	while(!cmdlist.empty()){
		splitlist = splitcommand(cmdlist,tracker);	
		if(splitlist.empty())
			break;
		if((strcmp(splitlist.at(0), "exit") == 0))
			exit(1);
		if(firstrun == true){
			prevcmd = execute(splitlist);
		}
		
		else if((tracker==1) && (prevcmd == true)){ //enter &&
			prevcmd = execute(splitlist);
		}
		else if(((tracker==2) == 0) && (prevcmd == false)){ //enter ||
			prevcmd = execute(splitlist);
		}
		else if(tracker==0){ //enter ;
			prevcmd = execute(splitlist);
		}

		splitlist.clear();
		firstrun = false;
	}

}

int main(int argc, char* argv[]){
	string command;

	while(command!="exit"){

		pout();
		getline(cin,command);
		workcommand(command);

	}
	
	return 0;	

}

