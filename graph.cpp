#include <cstring>
#include <iostream>
#include <queue>
#include <cilk/cilk.h>
#include <cilk/reducer.h>
#include <cilk/reducer_vector.h>
#include "graph.h"
#include "bag.cpp"

Graph::Graph(int V) {
	this->V = V;
	this->adj = new std::list<int>[this->V];
}

void Graph::addEdge(int v, int w) {
	this->adj[v].push_back(w);
	this->adj[w].push_back(v);
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
		for (std::list<int>::const_iterator it = this->adj[current].begin(), end = this->adj[current].end(); it != end; it++) {
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

void Graph::PBFS(int s) {
	int parents[this->V];
	memset(&parents, 0xFF, sizeof(parents));

	std::vector<int> frontier;
	cilk::reducer< cilk::op_vector<int> > new_frontier;

	parents[s] = s;
	frontier.push_back(s);

	int level = 1;

	while (!frontier.empty()) {
		cilk_for (std::vector<int>::iterator f = frontier.begin(), end = frontier.end(); f != end; f++) {
			std::cout << "Level " << level << ". Neighbors for " << *f << ": ";
			for (std::list<int>::iterator it = this->adj[*f].begin(), end = this->adj[*f].end(); it != end; it++) {
				if (parents[*it] == -1) {
					parents[*it] = *f;
					std::cout << *it << " ";
				}
			}
			std::cout << std::endl;
		}

		cilk_for (std::vector<int>::iterator f = frontier.begin(), end = frontier.end(); f != end; f++) {
			for (std::list<int>::const_iterator it = this->adj[*f].begin(), end = this->adj[*f].end(); it != end; it++) {
				if (parents[*it] == *f) {
					new_frontier->push_back(*it);
				}
			}
		}

		frontier.clear();
		new_frontier.move_out(frontier);
		level++;
	}
}


void Graph::BAGPBFS(int s) {
	Bag x(300);
	x.insert_vertex(2);
	x.insert_vertex(3);
	x.insert_vertex(4);
	x.insert_vertex(5);
	x.insert_vertex(1);
	Bag y(300);
	y.insert_vertex(1);
	x.merge(&y);
	std::cout << "Merged x and y" << std::endl;
	x.insert_vertex(1);	
	Bag *z = x.split();
	std::cout << "Split x" << std::endl;	
	/// x.insert_vertex(1);
	x.print();
	y.print();
	z->print();
}
