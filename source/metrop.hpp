#ifndef _metrop_hpp_
#define _metrop_hpp_

#include "mc.hpp"
#include <random>
#include <cmath>
#include <numeric>
#include <tuple>
#include <vector>
# define PI           3.14159265358979323846  /* pi */

class metrop : public mc {
private:
	void starting_point();	// randomly chooses a starting point
// probability distribution
	std::normal_distribution<double> gauss;
	double normal_distribution(const std::vector<double> &mu, const double sigma, const std::vector<double> &X);	// returns value of normal distribution function of dimension dim
	double get_std_normal_1d();	// samples from standard normal distribution in 1-D with mu=0, sigma=1
	std::vector<double> get_normal(const std::vector<double> &mu, const double sigma);	// samples from normal distribution with mu and sigma

	std::vector<double> y;	// proposed next position
	void propose();	// proposes next position (gaussian with center x and proposal_width stddeviation) written to y
	double proposal_width;

	bool success();	// returns 1 if update is accepted, 0 otherwise
	bool y_inside_space();	// checks if proposed y is inside space

public:
	metrop(const unsigned int dim);
	metrop(const std::vector< std::pair<double, double> > &lims);
	void set_proposal_width(const double w);	// sets probability width of random walk
	void update();				// generates a new random vector
};

#endif // !_metrop_hpp_