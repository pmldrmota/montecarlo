#ifndef _rd_hpp_
#define _rd_hpp_

#include <random>
#include <chrono>		// seed
#include "mc.hpp"

class rd : public mc {
private:
	std::mt19937 gen;
	std::uniform_real_distribution<double> distribution;
	double get();

public:
	rd(const unsigned int dim);
	rd(const unsigned int dim, const unsigned seed);	// constructor with user-defined seed
	void update();				// generates a new random vector
};

#endif // !_rd_hpp_
