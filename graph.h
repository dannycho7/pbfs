#ifndef GRAPH_H
#define GRAPH_H
#include <list>
#include "bag.cpp"
#include "bag_reducer.cpp"

class Graph {
public:
	Graph(int V);
	void addEdge(int v, int w);
	void BFS(int s);
	void PBFS(int s);
	void BAGPBFS(int s);
	void processLevelBag(Bag *&frontier, Bag_reducer &new_frontier, int *visited);
private:
	std::list<int> *adj;
	int V;
};

#endif // GRAPH_H
