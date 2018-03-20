#include <algorithm>
#include <cstring>
#include <fstream>
#include <iostream>
#include <queue>
#include <sstream>
#include <string>
#include <set>
#include "graph.h"
#include "example_util_gettime.h"

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
		if (edgeTupleSet.find(edgeTuple) == edgeTupleSet.end()) {
			std::istringstream iss(edgeTuple);
			int a, b;
			iss >> a >> b;

			if (a == b) continue;

			edgeTupleSet.insert(edgeTuple);
			// std::cout << "Adding " << edgeTuple << std::endl;
			g.addEdge(a, b);
		}
	}

	edgesFile.close();
}

int main(int argc, char **argv) {
	if (argc < 4) {
		std::cerr << "Correct Usage: ./bfs <algorithm_choice> <num_vertices> <edges_filename>" << std::endl;
		exit(1);
	}

	int algorithm_choice = atoi(argv[1]);

	if (algorithm_choice < 0 || algorithm_choice > 3) {
		std::cerr << "algorithm_choice must be in { 0, 1, 2 }" << std::endl;
		exit(1);
	}

	Graph g(atoi(argv[2]));
	readEdges(argv[3], g);

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
