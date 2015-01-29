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
string filterstr(string userin){

	//first remove comments
	string newstr = userin.substr(0, userin.find('#'));


	//checks beginning and end of string for hanging characters
	if(!newstr.empty()){
		while(newstr[0]==' '){
				newstr.erase(0,1);	
			}
		if(newstr[0]==newstr[0]==';' || newstr[0]=='|' || newstr[0]=='&'){
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
	
	return newstr;
}

vector<string> sepstring(string userin){
	vector<string> parsed;



	
}

int main(int argc, char* argv[]){
	
	string command;

	while(command!="exit"){

		pout();
		getline(cin,command);
//convert cmd into a cstring
//	cmd = command.c_str(); doesnt work const error.
		command = filterstr(command);

		pout()<<command<<endl;





	}
		return 0;	


}
