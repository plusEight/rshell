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

void analyzeflag(queue<char*> args){
	char* dirName;
	dirent *direntp; //pointer to each file in directory(this is a struct)
	bool l = false;
	bool r = false;

	DIR *dirp = NULL;
	
	dirName = ".";
	
	//if(notflag(args.front()) && (args.front()!=NULL))
	//	dirName = args.front();

	if(args.empty()){
		dirp = opendir(dirName);

		while ((direntp = readdir(dirp))){
				if(direntp->d_name[0]!='.')
					cout << direntp->d_name << endl;
			}
		
		closedir(dirp);
	}

	while(!args.empty()){
		if(args.front()[0]!='-'){ //not a flag so must be a path
			l = false;
			r = false;

			dirName = args.front();

			args.pop(); //ok, it is a path, now lets move on

		}

		else{ // it is a flag so...
			if(args.front()==string("-a")){
				dirp = opendir(dirName);
				if(dirp==NULL){
					perror("Error with Dir");
					exit(1);
				}
				while ((direntp = readdir(dirp)))  // use stat here to find attributes of file
					cout << direntp->d_name << endl;
			}

			if(args.front()==string("-l"))
				l=true;	

			if(args.front()==string("-r"))
				r=true;	

			args.pop();
		}

			closedir(dirp); //must close because it is a directory


	}

}

int main(int argc, char* argv[])
{
	queue<char*> q;

	q = loadin(argc, argv);
	analyzeflag(q);

	return 0;
}


//current problems:
//
//-a gives segfault
