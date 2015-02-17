CPPFLAGS = -ansi -pedantic -Wall -Werror
FILEPATH = ./src/ls.cpp
all:
	mkdir -p ./bin
	g++ $(CPPFLAGS) $(FILEPATH) -o ./bin/ls
	g++ $(CPPFLAGS) ./src/rshell.cpp -o ./bin/rshell

rshell:
	mkdir -p ./bin
	g++ $(CPPFLAGS) ./src/rshell.cpp -o ./bin/rshell

ls:
	mkdir -p ./bin
	g++ $(CPPFLAGS) $(FILEPATH) -o ./bin/ls
