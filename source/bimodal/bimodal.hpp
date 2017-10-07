#ifndef _bimodal_hpp_
#define _bimodal_hpp_

#include "common\mcmc.hpp"
#include <vector>
#include <random>
#include <cmath>
#include <tuple>
# define PI           3.14159265358979323846  /* pi */
#include "archives\archives.hpp"

class bimodal : public mcmc {
private:

	std::normal_distribution<double> gauss;
	double proposal_width;	// in percent of the span of a dimension
	void propose();	// proposes next position (gaussian distributed with center x and proposal_width*span/100 stddeviation)
	double get_std_normal_1d();

	double log_target_distribution(const double x);	// target distribution

	void set_log_p_success();
	bimodal_archive get_bimodal_archive();

public:
	bimodal(const unsigned int dim);
	bimodal(const std::vector< std::pair<double, double> > &lims);
	bimodal(bimodal_archive &ar);

	void set_proposal_width(const double w);	// sets probability width of random walk

	void archivise();
};

#endif // !_bimodal_hpp_