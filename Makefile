# Trivial makefile for the calculator scanner/parser.

# Note that the rule for the goal (parse)
# must be the first one in this file.

CPPFLAGS = -std=c++11
# Note that rule for goal (parse) must be the first one in this file.

parse: parse.o scan.o
	g++ $(CPPFLAGS) -o parse parse.o scan.o

clean:
	rm *.o parse

parse.o: scan.h
scan.o: scan.h
