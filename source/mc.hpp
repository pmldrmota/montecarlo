#ifndef _mc_class_h_
#define _mc_class_h_

#include <random>
#include <functional>	// std::bind
#include <chrono>		// seed

class mc {
private:
	unsigned int dim;	// dimension of space

public:
	mc(unsigned int dim);
	double ret_rd();
};

#endif // !_mc_class_h
