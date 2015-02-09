CPPFLAGS = -ansi -pedantic -Wall -Werror
FILEPATH = ./src/ls.cpp
all:
	mkdir -p ./bin
	g++ $(CPPFLAGS) $(FILEPATH) -o ./bin/ls

ls:
	mkdir -p ./bin
	g++ $(CPPFLAGS) $(FILEPATH) -o ./bin/ls
