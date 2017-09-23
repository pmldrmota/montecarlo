#ifndef _inference_hpp_
#define _inference_hpp_

#include "mcmc.hpp"
#include "triple.h"
#include <cmath>
#include <random>
#include <vector>
#include <tuple>
# define PI           3.14159265358979323846  /* pi */
#include "cereal\archives\binary.hpp"
#include "cereal\types\utility.hpp"
#include "cereal\types\vector.hpp"
#include <sstream>

enum dist_type { uniform, normal, logistic, exponential, chi_squared, lorentz, poisson };

struct inference_archive {
	/*
	// for re-construction of mc instance from cereal binary file: use constructor with this 'archive' structure as its argument
	*/
	mc_archive mcdata;

	std::vector<std::triple<dist_type, double, double>> prior_distributions;
	std::vector<std::vector<double>> observations;
	double proposal_width;

	// CEREAL	
	template<class Archive>
	void serialize(Archive & ar); // serialize things by passing them to the archive
};

class inference : public mcmc {
private:
	std::vector<std::triple<dist_type, double, double>> prior_distributions;
	unsigned dim_data;
	std::vector<std::vector<double>> observations;
	
	std::normal_distribution<double> gauss;
	double proposal_width;	// in percent of the span of a dimension
	void propose();	// proposes next position (gaussian distributed with center x and proposal_width*span/100 stddeviation)

	double get_std_normal_1d();
	double log_prior_distribution(const unsigned d, const double wert);	// returns value of prior distribution of d'th parameter at value wert
	double log_prior_p(const std::vector<double> &z);	// prior probability of having the parameter vector z (x or y)
	double log_normal_distribution(const std::vector<double> &data, const std::vector<double> &X);	// returns logarithmic normal probabilities of data respective to the parameter vector X (mu, sigma)
	double log_likelihood(const std::vector<double> &z);	// calculates the likelihood of all observations together given the parameter vector z (x or y). Calls neg_log_normal_distribution for each observation (problem specific!)
	
	void set_log_p_success();	// specified for inference purpose

public:
	inference(const unsigned dim);
	inference(const std::vector< std::pair<double, double> > &lims);
	inference(inference_archive &ar);

	void set_prior_distributions(const std::vector<std::triple<dist_type, double, double>> &pridist);	// TODO
	void set_proposal_width(const double w);	// sets probability width of random walk
	void set_observations(const std::vector<std::vector<double>> &observations);
	void add_observation(const std::vector<double> &observation);

	void archivise();
	inference_archive get_inference_archive();
};

#endif // !_inference_hpp_