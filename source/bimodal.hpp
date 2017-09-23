#ifndef _bimodal_hpp_
#define _bimodal_hpp_

#include "mcmc.hpp"
#include <vector>
#include <random>
#include <cmath>
#include <tuple>
# define PI           3.14159265358979323846  /* pi */

struct bimodal_archive {
	/*
	// for re-construction of mc instance from cereal binary file: use constructor with this 'archive' structure as its argument
	*/
	mc_archive mcdata;

	double proposal_width;

	// CEREAL	
	template<class Archive>
	void serialize(Archive & ar); // serialize things by passing them to the archive
};

class bimodal : public mcmc {
private:

	std::normal_distribution<double> gauss;
	double proposal_width;	// in percent of the span of a dimension
	void propose();	// proposes next position (gaussian distributed with center x and proposal_width*span/100 stddeviation)
	double get_std_normal_1d();

	double log_target_distribution(const double x);	// target distribution

	void set_log_p_success();

public:
	bimodal(const unsigned int dim);
	bimodal(const std::vector< std::pair<double, double> > &lims);
	bimodal(bimodal_archive &ar);

	void set_proposal_width(const double w);	// sets probability width of random walk

	void archivise();
	bimodal_archive get_bimodal_archive();
};

#endif // !_bimodal_hpp_