#include <algorithm>
#include <cstring>
#include <iostream>
#include <queue>
#include <stdio.h>
#include <string>
#include <set>
#include "graph.h"
#include "example_util_gettime.h"

void readEdges(Graph &g) {
	std::set <std::string> edgeTupleSet;
	std::string edgeTuple;

	int a, b;
	int numEdges = 0;

	while (scanf("%i %i", &a, &b) == 2) {
		if (a == b) continue;
		g.addEdge(a, b);
		numEdges++;
	}
}

int main(int argc, char **argv) {
	if (argc < 3) {
		std::cerr << "Correct Usage: ./bfs <algorithm_choice> <num_vertices> <edges_filename>" << std::endl;
		exit(1);
	}

	int algorithm_choice = atoi(argv[1]);

	if (algorithm_choice < 0 || algorithm_choice > 3) {
		std::cerr << "algorithm_choice must be in { 0, 1, 2 }" << std::endl;
		exit(1);
	}

	Graph g(atoi(argv[2]));
	readEdges(g);

	double t1 = example_get_time();
	switch (algorithm_choice) {
		case 0: g.BFS(1);
				break;
		case 1: g.PBFS(1);
				break;
		case 2: g.BAGPBFS(1);
				break;
	}
	double t2 = example_get_time();
	std::cout << "BFS time: " << t2 - t1 << "ms" <<  std::endl;
}
