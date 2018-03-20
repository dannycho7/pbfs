#include <cstring>
#include <iostream>
#include <queue>
#include <stack>
#include <cilk/cilk.h>
#include <cilk/reducer.h>
#include <cilk/reducer_vector.h>
#include "graph.h"
#include "bag.h"
#include "bag_reducer.h"

#define COARSENESS 2000

Graph::Graph(int V) {
	this->V = V;
	this->adj = new std::list<int>[this->V];
}

void Graph::addEdge(int v, int w) {
	this->adj[v].push_back(w);
}

void Graph::BFS(int s) {
	int levels[this->V];
	memset(&levels, 0xFF, this->V * sizeof(int));
	std::queue<int> frontier;

	int level = 0;
	frontier.push(s);
	levels[s] = level;

	int lastElInLevel = s;

	while (!frontier.empty()) {
		int current = frontier.front();
		frontier.pop();
		for (std::list<int>::const_iterator it = this->adj[current].begin(), end = this->adj[current].end(); it != end; it++) {
			if (levels[*it] < 0) {
				frontier.push(*it);
				levels[*it] = level + 1;
			}

		}

		if (current == lastElInLevel) {
			level++;
			lastElInLevel = frontier.back();
		}
	}

	for (int i = 0; i < this->V; i++) {
		std::cout << i << ": " << levels[i] << std::endl;
	}
}

void Graph::PBFS(int s) {
	int parents[this->V];
	int levels[this->V];
	memset(&parents, 0xFF, sizeof(int) * this->V);
	memset(&levels, 0xFF, sizeof(int) * this->V);

	std::vector<int> frontier;
	cilk::reducer< cilk::op_vector<int> > new_frontier;

	parents[s] = s;
	frontier.push_back(s);

	int level = 0;

	while (!frontier.empty()) {
		cilk_for (std::vector<int>::iterator f = frontier.begin(), end = frontier.end(); f != end; f++) {
			for (std::list<int>::iterator it = this->adj[*f].begin(), end = this->adj[*f].end(); it != end; it++) {
				if (parents[*it] == -1) {
					parents[*it] = *f;
				}
			}
		}

		cilk_for (std::vector<int>::iterator f = frontier.begin(), end = frontier.end(); f != end; f++) {
			levels[*f] = level;
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

	for (int i = 0; i < this->V; i++) {
		std::cout << i << ": " << levels[i] << std::endl;
	}
}

void Graph::processLevelBag(Bag *&frontier, Bag_reducer &new_frontier, int levels[], int level) {
	if (frontier->size() > COARSENESS) {
		Bag* y = frontier->split();
		cilk_spawn processLevelBag(y, new_frontier, levels, level);
		processLevelBag(frontier, new_frontier, levels, level);
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

					for (std::list<int>::iterator it = this->adj[current->vertex].begin(), end = this->adj[current->vertex].end(); it != end; it++) {
						if (levels[*it] < 0) {
							new_frontier.insert_vertex(*it);
							levels[*it] = level + 1;
						}
					}
				}
			}
		}
	}
}

void Graph::BAGPBFS(int s) {
	int levels[this->V];
	int *x;
	memset(&levels, 0xFF, this->V * sizeof(int));

	Bag* frontier = new Bag();
	Bag * buf;
	frontier->insert_vertex(s);
	int level = 0;
	levels[s] = level;


	while (!frontier->empty()) {
		Bag_reducer new_frontier;

		this->processLevelBag(frontier, new_frontier, levels, level);

		frontier->clear();
		frontier->merge(new_frontier.get_value());

		level++;
	}

	for (int i = 0; i < this->V; i++) {
		std::cout << i << ": " << levels[i] << std::endl;
	}
}
