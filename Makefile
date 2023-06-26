CC=g++

CFLAGS= --std=c++17

LDD= ./las-vegas/low-diameter-decomposition/

testDijkstra:
	$(CC) $(CFLAGS) test/testMain.cpp test/TestDijkstra.cpp -o test_dijkstra && ./test_dijkstra --success

runLDD:
	$(CC) $(CFLAGS) $(LDD)*.cpp -o ldd

clean: 
	rm -rf test_dijkstra