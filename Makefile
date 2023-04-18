CC=g++

CFLAGS= --std=c++17

testDijkstra:
	$(CC) $(CFLAGS) test/testMain.cpp test/TestDijkstra.cpp -o test_dijkstra && ./test_dijkstra --success

clean: 
	rm -rf test_dijkstra