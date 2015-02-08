#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <queue>
#include <deque>
#include <list>
#include <iomanip>
#include <algorithm>

#include <iostream>

using namespace std;

bool notflag(char* x ){
	if(x[0]=='-')
		return false;
	else
		return true;
}

queue<char*> loadin(int c, char* x[]) {
	queue<char*> q;
	
	for(int i=1; i<c; i++){
		q.push(x[i]);
	}
	
	return q;
}

void parseflags(char* flag, bool &a, bool &l, bool &r){
	int i=1;
	while(flag[i] != '\0'){
		if(flag[i]=='a')
			a = true;	
		else if(flag[i]=='l')
			l = true;	
		else if(flag[i]=='R')
			r = true;	
		else{
			cout <<"Invalid flag entered."<<endl;			
			exit(1);
		}

		i++;
	}
		
}

void formatprint(const char* dirName, bool a, bool l, bool r){

	vector<string> alph;
	dirent *direntp;

	if (dirName == NULL){
		dirName = ".";	
	}
	
	DIR *dirp = opendir(dirName);
	if (dirp == NULL)
		perror("Error with opendir");

	while ((direntp = readdir(dirp))){
		if (direntp->d_name[0] != '.'){
			alph.push_back(direntp->d_name);
		}
		if ((direntp->d_name[0] == '.') && (a == true)){
			alph.push_back(direntp->d_name);
		}
	}

	if (-1 == closedir(dirp))
		perror("error closing.");



//	if((a == false) && (l == false) && (r == false)){ //no flags
//		while ((direntp = readdir(dirp))){
//				if(direntp<0)
//					perror("error with readdir");
//				if(direntp->d_name[0]!='.')
//					alph.push_back(direntp->d_name);
//		}
//	}


	sort(alph.begin(), alph.end());
	for (int i=0; i<alph.size(); i++){
		cout << setw(10) << alph.at(i) << endl;
	}
	
}

void analyzeflag(queue<char*> args){
	char* dirName;
	dirent *direntp; //pointer to each file in directory(this is a struct)
	DIR *dirp = NULL;
	
	bool a = false;
	bool l = false;
	bool r = false;

	dirName = ".";
	
	//if(notflag(args.front()) && (args.front()!=NULL))
	//	dirName = args.front();

	if(args.empty()) {
			formatprint(dirName, a, l, r);
	}

	while(!args.empty()){
		if(args.front()[0]!='-'){ //not a flag so must be a path
			dirName = args.front();

			args.pop(); //ok, it is a path, now lets move on

		}

		else{ // it is a flag so...
			
			parseflags(args.front(), a, l, r);

			args.pop();

		}

		
		formatprint(dirName, a, l, r);
		//closedir(dirp); //should be handled by helper


	}


//TESTCASES
//	if (a == true){
//		cout << "a is true"<<endl;	
//	}
//	if (l == true){
//		cout << "l is true"<<endl;	
//	}
//	if (r == true){
//		cout << "R is true"<<endl;	
//	}
}

int main(int argc, char* argv[])
{
	queue<char*> q;

	q = loadin(argc, argv);
	analyzeflag(q);

	return 0;
}


//current problems:
//-a repeats normal ls
//invalid flag leads to segfault
