#include <algorithm>
#include <cstring>
#include <fstream>
#include <iostream>
#include <queue>
#include <sstream>
#include <string>
#include <set>
#include <cilk/cilk.h>
#include <cilk/reducer.h>
#include <cilk/reducer_vector.h>
#include "graph.h"
#include "example_util_gettime.h"

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

void readEdges(std::string filename, Graph &g) {
	std::ifstream edgesFile;
	edgesFile.open(filename.c_str());
	std::set <std::string> edgeTupleSet;

	if (!edgesFile) {
		std::cerr << "Unable to open " << filename << std::endl;
		exit(1);
	}

	std::string edgeTuple;

	while (std::getline(edgesFile, edgeTuple)) {
		std::istringstream iss(edgeTuple);
		std::stringstream orderedTuple;
		int a, b;
		iss >> a >> b;

		if (a == b) continue;

		int smaller = std::min(a, b);
		int bigger = std::max(a ,b);

		orderedTuple << smaller << " " << bigger;
		if (edgeTupleSet.find(orderedTuple.str()) == edgeTupleSet.end()) {
			edgeTupleSet.insert(orderedTuple.str());
			std::cout << "Adding " << orderedTuple.str() << std::endl;
			g.addEdge(a, b);
		}
	}

	edgesFile.close();
}

int main(int argc, char **argv) {
	if (argc < 3) {
		std::cerr << "Correct Usage: ./bfs <num_vertices> <edges_filename>" << std::endl;
		exit(1);
	}

	Graph g(atoi(argv[1]));
	readEdges(argv[2], g);

	double t1 = example_get_time();
	g.PBFS(1);
	double t2 = example_get_time();
	std::cout << "BFS time: " << t2 - t1 << "ms" <<  std::endl;
}
