#ifndef GRAPH_H
#define GRAPH_H
#include <list>


class Graph {
public:
	Graph(int V);
	void addEdge(int v, int w);
	void BFS(int s);
	void PBFS(int s);
	void BAGPBFS(int s);
private:
	std::list<int> *adj;
	int V;
};

#endif // GRAPH_H
