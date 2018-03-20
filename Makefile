all: bag.o graph.o bfs.o pennant.o
	icc -o ./bfs bag.o bfs.o graph.o pennant.o

pennant.o: pennant.cpp pennant.h node.h
	icc -o ./pennant.o -c ./pennant.cpp

bag.o: bag.cpp bag.h node.h pennant.o
	icc -o ./bag.o -c ./bag.cpp

bfs.o: bfs.cpp
	icc -o ./bfs.o -c ./bfs.cpp

graph.o: bag_reducer.h graph.cpp graph.h bag.o
	icc -o ./graph.o -c ./graph.cpp

clean:
	rm ./bfs *.o
