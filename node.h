#ifndef NODE_H
#define NODE_H

#include <cstdlib>

struct Node {
	int vertex;
	Node* left;
	Node* right;

	Node(int vertex) {
		this->vertex = vertex;
		this->left = NULL;
		this->right = NULL;
	}
};

#endif