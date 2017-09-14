#ifndef _mcmc_hpp_
#define _mcmc_hpp_

#include "mc.hpp"
#include <random>
#include <tuple>
#include <vector>

class mcmc : public mc {
protected:
// starting point
	std::uniform_real_distribution<double> uniform_dist;	// needed for starting_point() and success()
	void starting_point();	// randomly chooses a starting point
// proposal
	std::vector<double> y;	// proposed next position
	virtual void propose();	// proposes next position. has to be specified in inherited class!
// accepting proposal
	virtual bool success();	// returns 1 if update is accepted, 0 otherwise. has to be specified in inherited class!
	bool y_inside_space();	// checks if proposed y is inside space

public:
	mcmc(const unsigned int dim);
	mcmc(const std::vector< std::pair<double, double> > &lims);

	void update();				// proposes new position and accepts it if success evaluates to true
};

#endif // !_mcmc_hpp_