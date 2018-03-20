#ifndef PENNANT_H
#define PENNANT_H

#include "node.h"

class Pennant {
public:
	Pennant(Node* root);
	~Pennant();
	void p_merge(Pennant *&y);
	Pennant* p_split();
	static void FA(Pennant *&x, Pennant *&y, Pennant *&carry);

	Node *root;
};

#endif
