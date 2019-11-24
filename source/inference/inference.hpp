#ifndef _inference_hpp_
#define _inference_hpp_

#include "common/mcmc.hpp"
#include <cmath>
#include <random>
#include <vector>
#include <tuple>
# define PI           3.14159265358979323846  /* pi */
#include "archives/archives.hpp"
#include "common/dist_type.h"

typedef double(*log_model_distribution_PTR)(const std::vector<double>&, const std::vector<double>&);

class inference : public mcmc {
private:
	std::vector<std::tuple<dist_type, double, double>> prior_distributions;	// stores information about the prior distributions (type and parameter) - one for each variable
	std::vector<param_type> param_attributes;
	unsigned dim_data;
	std::vector<std::vector<double>> observations;

	std::normal_distribution<double> gauss;
	double proposal_width;	// in percent of the span of a dimension
	void propose();	// proposes next position (gaussian distributed with center x and proposal_width*span/100 stddeviation)

	double get_std_normal_1d();
	double log_prior_distribution(const unsigned d, const double wert);	// returns value of prior distribution of d'th parameter at value wert
	double log_prior_p(const std::vector<double> &z);	// prior probability of having the parameter vector z (x or y)
	log_model_distribution_PTR log_model_distribution;	// pointer to the logarithmic probability distribution of our model. Problem specific! Should return the logarithmic probability given an observation and the current parameters (in x or y).
	//double log_normal_distribution(const std::vector<double> &data, const std::vector<double> &X);	// MODEL: returns logarithmic normal probabilities of data respective to the parameter vector X (mu, sigma)
	double log_likelihood(const std::vector<double> &z);	// calculates the likelihood of all observations together given the parameter vector z (x or y). Calls neg_log_normal_distribution for each observation (problem specific!)

	void set_log_p_success();	// specified for inference purpose
	inference_archive get_inference_archive();

public:
	inference(const unsigned dim, log_model_distribution_PTR log_model_distribution);
	inference(const std::vector< std::pair<double, double> > &lims, log_model_distribution_PTR log_model_distribution);
	inference(inference_archive &ar, log_model_distribution_PTR log_model_distribution);

	void set_prior_distributions(const std::vector<std::tuple<dist_type, double, double>> &pridist);	// sets the types and parameters for each prior distribution (one for each variable)
	void set_proposal_width(const double w);	// sets probability width of random walk
	void set_param_attributes(const std::vector<param_type> &types); // sets whether each variable is periodic or linear
	void set_observations(const std::vector<std::vector<double>> &observations);	// over-writes current observations
	void add_observation(const std::vector<double> &observation);	// adds additional observations (e.g. for observation-by-observation initialisation)

	void archivise();
};

#endif // !_inference_hpp_
