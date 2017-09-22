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
	void make_step();				// generates a new random vector

public:
	rd(const unsigned int dim);
	rd(const std::vector< std::pair<double, double> > &lims);
	rd(mc_archive &ar);
};

#endif // !_rd_hpp_
