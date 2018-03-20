#ifndef BAG_REDUCER_H
#define BAG_REDUCER_H

#include "bag.h"
#include <cilk/cilk.h>
#include <cilk/reducer.h>

// Make the bag a reducer hyperobject
class Bag_reducer {

public:
	struct Monoid:cilk::monoid_base<Bag> {
		static void reduce(Bag *left, Bag *right) {
			left->merge(right);
		}
	};

	Bag_reducer() {
		this->imp_();
	} 

	Bag* get_value() {
		return &this->imp_.view();
	}

	void merge(Bag_reducer* br) {
		this->imp_.view().merge(&br->imp_.view());
	}

	void insert_vertex(int x) {
		imp_.view().insert_vertex(x);
	}

	void clear() {
		imp_.view().clear();
	}

	bool empty() {
		return imp_.view().empty();
	}

	void set_value(Bag *y) {
		this->imp_.set_value(*y);
	}

private:
	cilk::reducer<Monoid> imp_;
};

#endif
