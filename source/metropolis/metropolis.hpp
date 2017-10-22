#ifndef _metropolis_hpp_
#define _metropolis_hpp_

#include "common\mcmc.hpp"
#include <vector>
#include <random>
#include <cmath>
#include <tuple>
# define PI           3.14159265358979323846  /* pi */
#include "archives\archives.hpp"

typedef double(*log_target_distribution_PTR)(const std::vector<double>&);

class metropolis : public mcmc {
private:

	std::normal_distribution<double> gauss;
	double proposal_width;	// in percent of the span of a dimension
	void propose();	// proposes next position (gaussian distributed with center x and proposal_width*span/100 stddeviation)
	double get_std_normal_1d();

	//this functions needs to be external!
	log_target_distribution_PTR log_target_distribution;	// target distribution

	void set_log_p_success();	// return log_target_distribution(y) - log_target_distribution(x);

	metropolis_archive get_metropolis_archive();

public:
	metropolis(const unsigned int dim, log_target_distribution_PTR target_distribution);
	metropolis(const std::vector< std::pair<double, double> > &lims, log_target_distribution_PTR target_distribution);
	metropolis(metropolis_archive &ar, log_target_distribution_PTR target_distribution);

	void set_proposal_width(const double w);	// sets probability width of random walk

	void archivise();
};

#endif // !_metropolis_hpp_