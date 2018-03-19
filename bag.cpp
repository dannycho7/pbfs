#include <cmath>
#include <cstdlib>
#include <iostream>
#include "graph.h"

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

class Pennant {
public:
	Pennant(Node* root) {
		this->root = root;
	}

	~Pennant() {
		if (this->root != NULL) {
			std::cerr << "Implement Destructor" << std::endl;
			exit(1);
		}
	}

	void p_merge(Pennant *&y) {
		if (y == NULL) return;

		y->root->right = this->root->left;
		this->root->left = y->root;
		y->root = NULL;
		delete y;
		y = NULL;
	}

	Pennant* p_split() {
		Pennant *y = new Pennant(this->root->left);
		this->root->left = y->root->right;
		y->root->right = NULL;
		return y;
	}

	static void FA(Pennant *&x, Pennant *&y, Pennant *&carry) {
		if (x == NULL && y == NULL && carry == NULL) {
			return;
		}
		if (x != NULL && y == NULL && carry == NULL) {
			return;
		}
		if (x == NULL && y != NULL && carry == NULL) {
			x = y;
			y = NULL;
			return;
		}
		if (x == NULL && y == NULL && carry != NULL) {
			x = carry;
			carry = NULL;
			return;
		}
		if (x != NULL && y != NULL && carry == NULL) {
			x->p_merge(y);
			carry = x;
			x = NULL;
			return;
		}
		if (x != NULL && y == NULL && carry != NULL) {
			x->p_merge(carry);
			carry = x;
			x = NULL;
			return;
		}
		if (x == NULL && y != NULL && carry != NULL) {
			y->p_merge(carry);
			carry = y;
			y = NULL;
			return;
		}
		if (x != NULL && y != NULL && carry != NULL) {
			carry->p_merge(y);
			return;
		}
	}

	Node *root;
};

class Bag {
public:
	Bag() {
		this->backbone = new Pennant*[20]();
		this->r = 20;
	}

	Bag(int r) {
		this->backbone = new Pennant*[r]();
		this->r = r;
	}
	
	void insert_vertex(int x) {
		Node* vertex = new Node(x);
		Pennant *vertices = new Pennant(vertex);
		this->insert(vertices);
	}

	void insert(Pennant *&vertices) {
		int i = 0;
		while (this->backbone[i] != NULL) {
			this->backbone[i]->p_merge(vertices);
			vertices = this->backbone[i];
			this->backbone[i] = NULL;
			i++;
		}

		std::cout << "Value of i: " << i << std::endl;		
	
		this->backbone[i] = vertices;
	}

	void merge(Bag* y) {
		Pennant *carry = NULL;
		for (int i = 0; i < this->r; i++) {
			Pennant::FA(this->backbone[i], y->backbone[i], carry);
		}
	}

	Bag* split() {
		Bag *bag2 = new Bag(this->r);
		Pennant *first_elem = this->backbone[0];
		this->backbone[0] = NULL;
			
		for (int i = 1; i < this->r; i++) {
			if (this->backbone[i] != NULL) {
				bag2->backbone[i - 1] = this->backbone[i]->p_split();
				this->backbone[i - 1] = this->backbone[i];
				this->backbone[i] = NULL;
			}
		}

		if (first_elem != NULL) {
			this->insert(first_elem);
		}
		return bag2;
	}

	void print() {
		std::cout << "Bag: { ";
		for (int i = 0; i < this->r; i++) {
			std::cout << (this->backbone[i] != NULL) * pow(2, i) << " ";
		}
		std::cout << "}" << std::endl;
	}

	Pennant** backbone;
private:
	int r;
};
