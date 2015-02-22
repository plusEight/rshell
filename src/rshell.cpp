// Chuanping "Ping" Fan
// 2015 cfan002@ucr.edu

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
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

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
		if(newstr[0]=='>' || newstr[0]=='<' || newstr[0]=='|' || newstr[0]=='&'){
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
		if(newstr.at(newstr.size()-1)==';' || newstr.at(newstr.size()-1)=='|' || newstr.at(newstr.size()-1)=='&'|| newstr.at(newstr.size()-1)=='<'|| newstr.at(newstr.size()-1)=='>'){
			pout()<<"\'"<<newstr.at(newstr.size()-1)<<"\' "<< "cannot be at the end of your command."<<endl;
			return "";
		}

	}

	boost::replace_all(newstr, "<", " < ");
	boost::replace_all(newstr, ">", " > ");
	boost::replace_all(newstr, ">  >", " >> ");
	boost::replace_all(newstr, "|", " | ");
	boost::replace_all(newstr, "|  |", " || ");
	boost::replace_all(newstr, "&&", " && ");
	boost::replace_all(newstr, ";", " ; ");

	return newstr;
}

bool execute(const vector<char*> cmdlist, const int track, vector<char*> &cmdlist2, const int after){
	int sz = cmdlist.size();
	int stat;
	int saveout;
	int savein;
	int newfile;
	char **cmds = new char*[sz+1];
	for(int i=0; i<sz; i++){
		cmds[i] = cmdlist[i];
	}
	cmds[sz] = '\0';

	if((strcmp(cmdlist[0], "exit") == 0))
		exit(1);

	int pid = fork();
	if(pid<0){
		perror("Forking Error");
	}
	else if(pid==0){
		if (track == 5){
			if((savein = dup(0)) == -1)
				perror("error with dup");
			if(access(cmdlist2.at(0),F_OK) == -1)
				perror("error accessing file");
			if((newfile = open(cmdlist2.at(0), O_RDONLY))==-1)
				perror("error with open");
			if((dup2(newfile,0))==-1)
				perror("error with dup2");
		}
		if (after > 0){
			if((saveout = dup(1))==-1)
				perror("error with dup");
			if(access(cmdlist2.at(after+1),F_OK) != -1) //does output file exist?
					newfile = open(cmdlist2.at(after+1), O_WRONLY | O_TRUNC, 00744);
			else
				newfile = open(cmdlist2.at(after+1), O_WRONLY | O_CREAT, 00744);
			
			if(newfile == -1){
				perror("error with open");
			}
			if((dup2(newfile,1))==-1)
				perror("error with dup2");

		}
		if(track == 3 || track == 4){ 	// > or >>
			if((saveout = dup(1))==-1)
				perror("error with dup");
			if(access(cmdlist2.at(0),F_OK) != -1){  //does output file exist?
				if(track == 4)
					newfile = open(cmdlist2.at(0), O_WRONLY | O_APPEND, 00744);
				else
					newfile = open(cmdlist2.at(0), O_WRONLY | O_TRUNC, 00744);
			}
			else
				newfile = open(cmdlist2.at(0), O_WRONLY | O_CREAT, 00744);
			
			if(newfile == -1){
				perror("error with open");
			}
			if((dup2(newfile,1))==-1)
				perror("error with dup2");
		}
		//**************execute here
		if(execvp(cmds[0], cmds)==-1){
			perror("execvp error");
		}
		//**************execute here
		if ((track == 3) || (track == 4) || (after > -1)){
			dup2(saveout, 1);
			if(-1==close(newfile))
				perror("error closing");
			
		}

		if(track == 5){
			dup2(savein, 0);
			if(-1==close(newfile))
				perror("error closing");
		}
		exit(1);
	}
	else{
		if(waitpid(pid,&stat,0)==-1){
			perror("error executing");
			exit(1);
		}	
	}
	
	if(after>0){
	}
	delete [] cmds;
	if (stat == 0)
		return true;
	else 
		return false;
}

bool adjconnector(const vector<char*> x){
	vector<string> connectors;
	bool firstcon = false;
	bool secondcon = false;
	connectors.push_back("&&");
	connectors.push_back("||");
	connectors.push_back(">");
	connectors.push_back(">>");
	connectors.push_back("<");
	connectors.push_back("|");

	if(x.size()>1){
		for (size_t i=0;i<connectors.size();i++){
			if (x.at(1) == connectors.at(i))
				firstcon = true;
			if (x.at(0) == connectors.at(i))
				secondcon = true;
		}
	}

	if(firstcon&&secondcon)
		return false;

	return true;
}

vector<char*> splitcommand(vector<char*> &x, int &y){
	vector<char*> lhs;

	size_t i=0;
	if (x.empty())
		return lhs;
	if(x.size() > 1){

		if (!adjconnector(x)){
			cerr << "Cannot have adjacent connectors!" << endl;
			return lhs;
		}

		if((strcmp(x.at(0),";")==0)){
			x.erase(x.begin());
			y = 0;
		}
		if((strcmp(x.at(0),"&&")==0)){
			x.erase(x.begin());
			y = 1;
		}
		if((strcmp(x.at(0),"||")==0)){
			x.erase(x.begin());
			y = 2;
		}

	}

	for (i=0; i<x.size(); i++){
		if((strcmp(x.at(i),"||")==0) || (strcmp(x.at(i),"&&")==0) || (strcmp(x.at(i),";")==0) || (strcmp(x.at(i),">")==0) || 
				(strcmp(x.at(i),">>")==0) || (strcmp(x.at(i),"<")==0) || (strcmp(x.at(i),"|")==0))
			break;
		lhs.push_back(x.at(i));
	}

	x.erase(x.begin(), x.begin()+i);

	if(x.size()>1){
		if((strcmp(x.at(0),">")==0)){
			x.erase(x.begin());
			y = 3;
		}
		if((strcmp(x.at(0),">>")==0)){
			x.erase(x.begin());
			y = 4;
		}
		if((strcmp(x.at(0),"<")==0)){
			x.erase(x.begin());
			y = 5;
		}
		if((strcmp(x.at(0),"|")==0)){
			x.erase(x.begin());
			y = 6;
		}	
	}

	return lhs;
}

int checkclose(const vector<char*> x){
	vector<string> connectors;
	connectors.push_back("&&");
	connectors.push_back("||");
	connectors.push_back("<");

	if(x.size()>1){
		for (size_t i=0;i<x.size();i++){
			for (size_t j=0; j<connectors.size(); j++){
				if(x.at(i) == connectors.at(j)){
					break;
				}
				else if(strcmp(x.at(i), ">")==0)
					return i;
				else if(strcmp(x.at(i), ">>")==0)
					return i;
			}
		}
	}


	return -1;
}


void workcommand(const string userin){
	string input = filterstr(userin);
	vector<char*> cmdlist = parsestring(input);
	vector<char*> splitlist;
	int tracker = -1;
	int after = -1;
	bool prevcmd = true; //prev command succeeded or failed
	bool firstrun = true;

	while(!cmdlist.empty()){
		splitlist = splitcommand(cmdlist,tracker);	
		if(splitlist.empty())
			break;

		if(firstrun == true){
			if(tracker== 3 || tracker == 4 || tracker == 5){
				if(tracker == 5)
					after = checkclose(cmdlist);

				prevcmd = execute(splitlist, tracker, cmdlist, after);
				cmdlist.erase(cmdlist.begin());
			}
			else
				prevcmd = execute(splitlist, tracker, cmdlist, after);
		}
		else{
			if((tracker==1) && (prevcmd == true)){ //enter &&
				prevcmd = execute(splitlist, tracker, cmdlist, after);
			}
			else if(((tracker==2)) && (prevcmd == false)){ //enter ||
				prevcmd = execute(splitlist, tracker, cmdlist, after);
			}
			else if(tracker==0){ //enter ;
				prevcmd = execute(splitlist, tracker, cmdlist, after);
			}
			else if(tracker== 3 || tracker == 4 || tracker == 5){ //enter > >> <
				if(tracker == 5)
					after = checkclose(cmdlist);
				prevcmd = execute(splitlist, tracker, cmdlist, after);
				cmdlist.erase(cmdlist.begin());
			}
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

