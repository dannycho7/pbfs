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
	g.BAGPBFS(1);
	double t2 = example_get_time();
	std::cout << "BFS time: " << t2 - t1 << "ms" <<  std::endl;
}
