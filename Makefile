CC=g++
FLAGS=-std=c++11 -Wall -Wdeprecated -pedantic -O2 
TEST=gtest.out
TESTOS=bin/TestGraph.o
HEADERS=src/graph_adj.h src/graph_edge.h
BINARY=

$(TEST): $(OBJECTS) $(TESTOS)
	$(CC) test/gtest_main.cc -o bin/$@ -lgtest $(TESTOS)

bin/TestGraph.o: test/TestGraph.cpp $(HEADERS)
	$(CC) $(FLAGS) $< -c -o $@

clean:
	rm -f $(OBJECTS) $(TEST) $(TESTOS) bin/*

all: $(TEST)
