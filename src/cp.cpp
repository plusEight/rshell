#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <unistd.h>
#include "Timer.h"

using namespace std;

void readwrite(char *argv[]){
    struct stat sb;
    struct stat sb2;
    
    char buff;
    
    int save = open(argv[1],O_RDONLY);

    if (stat(argv[1], &sb) == -1) { //check if file is good
        perror("stat");
        exit(EXIT_FAILURE);
    }
    
    int save2;
    
    if(stat(argv[2], &sb2)!=-1){ //if the output file already exists
        save2 = open(argv[2],O_WRONLY | O_TRUNC);
    }
    else save2 = open(argv[2],O_WRONLY | O_CREAT);
    
    
    if (stat(argv[2], &sb2) == -1) { //check if file is good
        perror("stat2");
        exit(EXIT_FAILURE);
    }
    
    while(read(save,&buff,1)){          // loop getting single characters
        //cout << "Hey hey hey" << endl;
        write(save2,&buff,1);
    }
}

void getput(char *argv[]){
    //char str[256];

  //std::cout << "Enter the name of an existing text file: ";
  //std::cin.get (str,256);    // get c-string
  
  struct stat sb;
  struct stat sb2;

  if (stat(argv[1], &sb) == -1) { //check if input file is good
        perror("stat");
        exit(EXIT_FAILURE);
  }

  if (stat(argv[2], &sb2) == -1) { //check if output file is good
        perror("stat2");
        exit(EXIT_FAILURE);
  }

  std::ifstream is(argv[1]);     // open file

  if(!is.good()) {
    std::cerr << "Input stream was not good";
  }
  
  std::ofstream outfile(argv[2]);
  
  if(!outfile.good()) {
    std::cerr << "Output stream was not good";
  }

  char c;
  while (is.get(c))          // loop getting single characters
    outfile.put(c);

  is.close();                // close file
  outfile.close();

}

void readwritebuffer(char *argv[]){
    struct stat sb;
    struct stat sb2;
    
    char buff[BUFSIZ];
    
    int save = open(argv[1],O_RDONLY);

    if (stat(argv[1], &sb) == -1) { //check if file is good
        perror("stat");
        exit(EXIT_FAILURE);
    }
    
    int save2;
    
    if(stat(argv[2], &sb2)!=-1){ //if the output file already exists
        save2 = open(argv[2],O_WRONLY | O_TRUNC);
    }
    else save2 = open(argv[2],O_WRONLY | O_CREAT);
    
    
    if (stat(argv[2], &sb2) == -1) { //check if file is good
        perror("stat2");
        exit(EXIT_FAILURE);
    }
    
    int test;
    
    while((test = read(save,&buff,BUFSIZ))){          // loop getting single characters
        write(save2,&buff,test);
    }
}



int main(int argc, char* argv[]){
    if(argv[3]!=NULL){
        Timer t1;
        Timer t2;
        Timer t3;
        
        double time1;
        double time2;
        double time3;
        
        t1.start();
        getput(argv);
        t1.elapsedUserTime(time1);
        cout << "The time for getout is: " << time1 << endl;
        
        t2.start();
        readwrite(argv);
        t2.elapsedUserTime(time2);
        cout << "The time for readwrite is: " << time2 << endl;
        
        t3.start();
        readwrite(argv);
        t3.elapsedUserTime(time3);
        cout << "The time for readwrite with buffer is: " << time3 << endl;
    }
    else{
        cout<<"Using getput... "<<endl;
        getput(argv);
    }
    
    return 0;
}
