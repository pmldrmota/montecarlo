#ifndef _inference_hpp_
#define _inference_hpp_

#include "mcmc.hpp"
#include <cmath>
#include <random>
#include <vector>
#include <tuple>
# define PI           3.14159265358979323846  /* pi */

class inference : public mcmc {
private:
	std::vector<double> prior;

	std::normal_distribution<double> gauss;
	double proposal_width;	// in percent of the span of a dimension
	void propose();	// proposes next position (gaussian distributed with center x and proposal_width*span/100 stddeviation)

	double get_std_normal_1d();
	double neg_log_prior_distribution(const std::vector<double> &data);	// returns value of prior distribution
	double neg_log_normal_distribution(const std::vector<double> &data, const std::vector<double> &X);	// returns negative logarithmic normal probabilities of data respective to X
	double neg_log_likelihood(const std::vector<std::vector<double>> &observations, const std::vector<double> &z);	// calculates the likelihood of all observations together respective state z (x or y)
	
	bool success(const std::vector<std::vector<double>> &observations);	// specified for inference purpose

public:
	inference(const unsigned int dim);
	inference(const std::vector< std::pair<double, double> > &lims);
	inference(const std::vector< std::pair<double, double> > &lims, const std::vector<double> &prior);

	void set_prior(const std::vector<double> &v);
	void set_proposal_width(const double w);	// sets probability width of random walk
};

#endif // !_inference_hpp_