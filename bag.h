#ifndef BAG_H
#define BAG_H

#include "pennant.h"

class Bag {
public:
	Bag();
	Bag(int r);
	~Bag();
	void clear();
	void init(int r);
	void insert_vertex(int x);
	void insert(Pennant *&vertices);
	void merge(Bag* y);
	Bag* split();
	void print();
	int size();
	bool empty();

	Pennant** backbone;
	int largest_nonempty_index;
private:
	int r;
};

#endif
