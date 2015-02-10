#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <cstring>
#include <queue>
#include <deque>
#include <list>
#include <iomanip>
#include <algorithm>
#include <sys/stat.h>
#include <linux/stat.h>
#include <grp.h>
#include <pwd.h>
#include <time.h>
#include <unistd.h>


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
		
	if(i==1){
		cout <<"Invalid flag entered." << endl;
		exit(1);
	}
}

string getpermissions(mode_t modes){
	string def = "----------";

	//file types
	if(S_ISDIR(modes))
		def.at(0)='d';
	else if(S_ISLNK(modes))
		def.at(0)='l';
	else if(S_ISFIFO(modes))
		def.at(0)='p';
	else if(S_ISSOCK(modes))
		def.at(0)='s';
	else if(S_ISBLK(modes))
		def.at(0)='b';
	else if(S_ISCHR(modes))
		def.at(0)='c';

	if(modes & S_IRUSR)
		def.at(1) = 'r';	
	if(modes & S_IWUSR)
		def.at(2) = 'w';
	if(modes & S_IXUSR)
		def.at(3) = 'x';
	if(modes & S_IRGRP)
		def.at(4) = 'r';
	if(modes & S_IWGRP)
		def.at(5) = 'w';
	if(modes & S_IXGRP)
		def.at(6) = 'x';
	if(modes & S_IROTH)
		def.at(7) = 'r';
	if(modes & S_IWOTH)
		def.at(8) = 'w';
	if(modes & S_IXOTH)
		def.at(9) = 'x';
	
	return def;

}

string contpath(string s, string c){
	//c is path
	//s is filename
	return (c+"/"+s);
	
}

string thetime(struct stat x){
	string currtime;
	currtime = ctime(&x.st_mtime);

	return currtime;	
}

void formatprint(const char* dirName, bool &a, bool l, bool r){
	//debugging
	vector<string> alph;
	vector<string> laterdir;
	vector<const char*> converted;
	dirent *direntp;

	if (dirName == NULL){
		dirName = ".";	
	}
	
	DIR *dirp = opendir(dirName);
	if (dirp == NULL){
		perror("Error with opendir");
		exit(1);
	}
	while ((direntp = readdir(dirp))){
		if(direntp==NULL){
			perror("readdir error");
		}
		if (direntp->d_name[0] != '.'){
			alph.push_back(direntp->d_name);
		}
		if ((direntp->d_name[0] == '.') && (a == true)){ //if a is true
			alph.push_back(direntp->d_name);
		}
	}

	if (-1 == closedir(dirp))
		perror("error closing.");

	sort(alph.begin(), alph.end());

	int blocks = 0;
	for (size_t i=0; i<alph.size(); i++){
		struct stat mylogs;	

		if(-1 == stat(contpath(alph.at(i),dirName).c_str(), &mylogs)) //oath in cstr, log
			perror("error with stat");
		
		if(getpermissions(mylogs.st_mode).at(0)=='d'){ //if dir
			laterdir.push_back(alph.at(i));
		}

		if(l == true){
			struct passwd logininfo = *getpwuid(mylogs.st_uid);
			struct group groupinfo = *getgrgid(mylogs.st_gid);
			
			if(alph.at(i).size() >=20)
				cout <<setw(20)<<left<<alph.at(i).substr(0, 18)+'*';
			else
				cout << setw(20) <<left<<alph.at(i);
			cout << " " << getpermissions(mylogs.st_mode) <<" "<<mylogs.st_nlink<<" "<< logininfo.pw_name << " "<<groupinfo.gr_name;
			cout << " " << setw(7)<<left<<mylogs.st_size<<" "<<thetime(mylogs);
			blocks = blocks + mylogs.st_blocks;
		}


		else{	// -a and normal ls
			cout << alph.at(i) << " " ;
		}

	}
	
	if (l == true){
		cout<<"Total Blocks : "<< blocks << endl;
	}

	if (r==true&&(!laterdir.empty())){
		for(size_t i=0;i<laterdir.size(); i++){
			if((laterdir.at(i)!=".")&&(laterdir.at(i) != "..")){
				converted.push_back((laterdir.at(i)).c_str());
			}
		}

		for(size_t i=0;i<converted.size(); i++){
			cout<<endl<< endl <<contpath(converted.at(i), dirName)<< ": "<<endl;
			formatprint(contpath(converted.at(i), dirName).c_str(), a, l, r);
		}
	}
}

void analyzeflag(queue<char*> args){
	char* dirName=NULL;
	vector<char*> direc;

	bool a = false;
	bool l = false;
	bool r = false;

	if(args.empty()) {
			formatprint(dirName, a, l, r);
			cout << endl;
	}

	while(!args.empty()){
		if(args.front()[0]!='-'){ //not a flag so must be a path
			//formatting tests

			dirName = args.front();
			direc.push_back(args.front());
			args.pop(); //ok, it is a path, now lets move on

		}

		else{ // it is a flag so...
			
			parseflags(args.front(), a, l, r);

			args.pop();

		}

	}

	if(direc.size()==0 &&(a==true||l==true||r==true)){
		formatprint(dirName, a, l ,r);
		cout<<endl;
	}
	
	for (size_t i=0; i<direc.size(); i++){
		cout<<direc.at(i)<<": "<<endl;
		formatprint(direc.at(i), a, l, r);
		cout << endl;
	}

}

int main(int argc, char* argv[])
{
	queue<char*> q;

	q = loadin(argc, argv);
	analyzeflag(q);

	return 0;
}

