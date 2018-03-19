#include <cstring>
#include <iostream>
#include <queue>
#include <stack>
#include <cilk/cilk.h>
#include <cilk/reducer.h>
#include <cilk/reducer_vector.h>
#include "graph.h"
#include "bag.cpp"
#include "bag_reducer.cpp"

#define COARSENESS 7

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

void Graph::processLevelBag(Bag *&frontier, Bag_reducer &new_frontier, int *visited) {
	if (frontier->size() > COARSENESS) {
		Bag* y = frontier->split();
		cilk_spawn processLevelBag(y, new_frontier, visited);
		processLevelBag(frontier, new_frontier, visited);
		cilk_sync;
	} else {
		std::stack<Node *> nodes;

		for (int i = 0; i <= frontier->largest_nonempty_index; i++) {
			if (frontier->backbone[i] != NULL) {
				nodes.push(frontier->backbone[i]->root);
				while (nodes.size() > 0) {
					Node *current = nodes.top();
					nodes.pop();

					if (current->left != NULL) {
						nodes.push(current->left);
					}

					if (current->right != NULL) {
						nodes.push(current->right);
					}

					this->adj[current->vertex];

					for (std::list<int>::iterator it = this->adj[current->vertex].begin(), end = this->adj[current->vertex].end(); it != end; it++) {
						new_frontier.insert_vertex(*it);
					}
				}
			}
		}
	}
}

void Graph::BAGPBFS(int s) {
	int *visited = new int[this->V];
	memset(visited, 0, sizeof(visited));

	Bag* frontier = new Bag();
	Bag_reducer new_frontier;
	Bag * buf;
	visited[s] = s;
	frontier->insert_vertex(s);

	int level = 1;

	while (!frontier->empty()) {
		std::cout << "Start of loop" << std::endl;

		this->processLevelBag(frontier, new_frontier, visited);

		std::cout << "Finish process level" << std::endl;

		frontier->print();
		delete frontier;
		frontier = new_frontier.get_value();

		frontier->print();
		buf = new Bag();
		new_frontier.set_value(buf);
		level++;
	}

}
