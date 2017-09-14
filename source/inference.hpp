#ifndef _inference_hpp_
#define _inference_hpp_

#include "mcmc.hpp"
#include <cmath>
#include <random>
#include <vector>
#include <tuple>
# define PI           3.14159265358979323846  /* pi */

enum dist_type { uniform = 0, normal = 1 };

class inference : public mcmc {
private:
	dist_type prior_dist_type;
	std::vector<std::vector<double>> observations;
	
	std::normal_distribution<double> gauss;
	double proposal_width;	// in percent of the span of a dimension
	void propose();	// proposes next position (gaussian distributed with center x and proposal_width*span/100 stddeviation)

	double get_std_normal_1d();
	double neg_log_prior_distribution(const std::vector<double> &data);	// returns value of prior distribution - for the moment uniform distribution returning 0.
	double neg_log_normal_distribution(const std::vector<double> &data, const std::vector<double> &X);	// returns negative logarithmic normal probabilities of data respective to X
	double neg_log_likelihood(const std::vector<double> &z);	// calculates the likelihood of all observations together respective state z (x or y)
	
	bool success();	// specified for inference purpose

public:
	inference(const unsigned int dim);
	inference(const std::vector< std::pair<double, double> > &lims);

	void set_proposal_width(const double w);	// sets probability width of random walk
	void set_observations(const std::vector<std::vector<double>> &observations);
	void add_observation(const std::vector<double> &observation);
};

#endif // !_inference_hpp_