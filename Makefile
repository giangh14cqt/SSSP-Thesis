CC=g++

CFLAGS= --std=c++17

test:	test/TestDijkstra.cpp 
	$(CC) $(CFLAGS) test/TestDijkstra.cpp  -o test_dijkstra

clean: 
	rm -rf test_dijkstra