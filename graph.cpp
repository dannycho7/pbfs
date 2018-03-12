#include "graph.h"

Graph::Graph(int V) {
	this->V = V;
	adj = new std::list<int>[this->V];
}

void Graph::addEdge(int v, int w) {
	adj[v].push_back(w);
}

void Graph::BFS(int s) {
	
}

int main() {

}