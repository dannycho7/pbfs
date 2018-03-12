#include <iostream>
#include <cstring>
#include <queue>
#include "graph.h"

Graph::Graph(int V) {
	this->V = V;
	adj = new std::list<int>[this->V];
}

void Graph::addEdge(int v, int w) {
	adj[v].push_back(w);
	adj[w].push_back(v);
}

void Graph::BFS(int s) {
	bool visited[this->V];
	memset(visited, 0, this->V * sizeof(bool));
	std::queue<int> frontier;

	frontier.push(s);

	int level = 0;
	int lastElInLevel = s;

	while (!frontier.empty()) {
		int current = frontier.front();
		frontier.pop();
		visited[current] = true;
		std::cout << "level " << level << ": " << current << " adjacent edges: ";
		for (std::list<int>::const_iterator it = adj[current].begin(), end = adj[current].end(); it != end; it++) {
			if (!visited[*it]) {
				std::cout << *it << " ";
				visited[*it] = true;
				frontier.push(*it);
			}

		}
		std::cout << std::endl;

		if (current == lastElInLevel) {
			level++;
			lastElInLevel = frontier.back();
		}
	}


}

int main() {
	Graph g(5);
	g.addEdge(1, 2);
	g.addEdge(1, 3);
	g.addEdge(2, 4);
	g.BFS(1);
}
