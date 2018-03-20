#ifndef GRAPH_H
#define GRAPH_H
#include <list>
#include "bag.h"
#include "bag_reducer.h"

class Graph {
public:
	Graph(int V);
	void addEdge(int v, int w);
	void BFS(int s);
	void PBFS(int s);
	void BAGPBFS(int s);
	void processLevelBag(Bag *&frontier, Bag_reducer &new_frontier, int levels[], int level);
private:
	std::list<int> *adj;
	int V;
};

#endif // GRAPH_H
