all: bag.o graph.o bfs.o
	icc -o ./bfs bag.o bfs.o graph.o

bag.o: bag.cpp
	g++ -o ./bag.o -c ./bag.cpp

bfs.o: bfs.cpp
	g++ -o ./bfs.o -c ./bfs.cpp

graph.o: bag.cpp graph.cpp graph.h
	icc -o ./graph.o -c ./graph.cpp

clean:
	rm ./bfs *.o
