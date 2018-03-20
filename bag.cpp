#include <cmath>
#include <cstdlib>
#include <iostream>
#include "bag.h"
#include "node.h"
#include "pennant.h"

Bag::Bag() {
	this->init(20);
}

Bag::Bag(int r) {
	this->init(r);
}

void Bag::init(int r) {
	this->backbone = new Pennant*[r]();
	this->r = r;
	this->largest_nonempty_index = -1;
}
	
Bag::~Bag() {
	this->clear();
	delete[] this->backbone;
}

void Bag::clear() {
	for (int i = 0; i <= this->largest_nonempty_index; i++) {
		delete this->backbone[i];
		this->backbone[i] = NULL;
	}
	
	this->largest_nonempty_index = -1;
}

void Bag::insert_vertex(int x) {
	Node* vertex = new Node(x);
	Pennant *vertices = new Pennant(vertex);
	this->insert(vertices);
}

void Bag::insert(Pennant *&vertices) {
	int i = 0;
	while (this->backbone[i] != NULL) {
		this->backbone[i]->p_merge(vertices);
		vertices = this->backbone[i];
		this->backbone[i] = NULL;
		i++;
	}

	this->backbone[i] = vertices;
	if (i > largest_nonempty_index) {
		this->largest_nonempty_index = i;
	}
}

void Bag::merge(Bag* y) {
	int max_nonempty_index = std::max(y->largest_nonempty_index, this->largest_nonempty_index);
	Pennant *carry = NULL;

	for (int i = 0; i <= max_nonempty_index; i++) {
		Pennant::FA(this->backbone[i], y->backbone[i], carry);
	}

	if (carry != NULL) {
		max_nonempty_index += 1;
		this->backbone[max_nonempty_index] = carry;
	}

	this->largest_nonempty_index = max_nonempty_index;
}

Bag* Bag::split() {
	Bag *bag2 = new Bag(this->r);
	Pennant *first_elem = this->backbone[0];
	this->backbone[0] = NULL;
	
	for (int i = 1; i <= this->largest_nonempty_index; i++) {
		if (this->backbone[i] != NULL) {
			bag2->backbone[i - 1] = this->backbone[i]->p_split();
			this->backbone[i - 1] = this->backbone[i];
			this->backbone[i] = NULL;
		}
	}

	if (this->largest_nonempty_index != 0) {
		this->largest_nonempty_index = this->largest_nonempty_index - 1;
		bag2->largest_nonempty_index = this->largest_nonempty_index;
	}

	if (first_elem != NULL) {
		this->insert(first_elem);
	}
	return bag2;
}

void Bag::print() {
	std::cout << "Bag: { ";
	for (int i = 0; i < this->r; i++) {
		std::cout << (this->backbone[i] != NULL) * pow(2, i) << " ";
	}
	std::cout << "}" << std::endl;
}

int Bag::size() {
	int sum = 0;
	for (int i = 0; i <= this->largest_nonempty_index; i++) {
		if (this->backbone[i] != NULL) {
			sum += pow(2, i);
		}
	}

	return sum;
}

bool Bag::empty() {
	return (this->largest_nonempty_index < 0);
}
