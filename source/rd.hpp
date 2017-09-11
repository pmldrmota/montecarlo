#ifndef _rd_hpp_
#define _rd_hpp_

#include <random>
#include <tuple>
#include <vector>
#include "mc.hpp"

class rd : public mc {
private:
	std::uniform_real_distribution<double> distribution;
	double get(const unsigned int i);

public:
	rd(const unsigned int dim);
	rd(const std::vector< std::pair<double, double> > &lims);
	void update();				// generates a new random vector
};

#endif // !_rd_hpp_
