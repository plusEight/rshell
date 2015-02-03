#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <vector>
#include <string>

#include <iostream>

using namespace std;

int main(int argc, char* argv[])
{
    char* dirName;
	
	if(argv[1]==NULL)			
		dirName = ".";
	else
		dirName = argv[1];

    DIR *dirp = opendir(dirName);
	if(dirp==NULL){
		perror("Error with Dir");
		exit(1);
	}
    
	dirent *direntp;
		
	while ((direntp = readdir(dirp)))
        cout << direntp->d_name << endl;  // use stat here to find attributes of file
    closedir(dirp);


}

