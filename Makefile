CC=g++
FLAGS=-std=c++11 -Wall -Wdeprecated -pedantic -O2 
TEST=gtest.out
OBJECTS=bin/graph_adj.o bin/graph_edge.o
TESTOS=bin/TestGraph.o
HEADERS=
BINARY=

$(TEST): $(OBJECTS) $(TESTOS)
	$(CC) test/gtest_main.cc -o bin/$@ -lgtest $(OBJECTS) $(TESTOS)

bin/graph_adj.o: src/graph_adj.cpp $(HEADERS)
	$(CC) $(FLAGS) $< -c -o $@

bin/graph_edge.o: src/graph_edge.cpp $(HEADERS)
	$(CC) $(FLAGS) $< -c -o $@

bin/TestGraph.o: test/TestGraph.cpp $(HEADERS)
	$(CC) $(FLAGS) $< -c -o $@

clean:
	rm -f $(OBJECTS) $(TEST) $(TESTOS) bin/*

all: $(TEST)
