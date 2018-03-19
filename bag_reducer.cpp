#ifndef BAG_REDUCER
#define BAG_REDUCER

#include "bag.cpp"
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

private:
	cilk::reducer<Monoid> imp_;
};

#endif