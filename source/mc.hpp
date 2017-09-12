/* ----------
// Monte Carlo Class
// ----------
//
// taking the dimension of the space and the algorithm to be used for the walker
//	- randomly distributed points
//	- metropolis-hastings
//	- gibbs
// sequentially returns sampling points in the space
//
//----------
// Author: Peter Drmota, BSc
// ----------
*/
#ifndef _mc_hpp_
#define _mc_hpp_

#include <iostream>
#include <vector>
#include <tuple>
#include <cmath>
#include <numeric>
#include <random>
#include <chrono>		// seed

class mc {
protected:
// SPACE VARIABLES
	unsigned int dim;		// dimension of space
	std::vector< std::pair<double, double> > limits;	// limits of the space
	std::vector<double> spans;	// span of the dimensions of the space
	double volume;	// volume of the space
	void complement_space_vars();	// fill spans and volume
// ALGORITHM VARIABLES
	unsigned int step_nr;	// number of steps already done
	std::vector<double> x;	// contains current position
// RANDOM GENERATOR VARIABLES
	std::mt19937 gen;		// random number generator needed for all distributions

public:
// CONSTRUCTOR
	mc(const unsigned int dim);
	mc(const std::vector< std::pair<double, double> > &lims);
// RETURN VARIABLES
	unsigned int dimension();		// returns dim
	unsigned int get_step_nr();		// returns nr_steps
	std::pair<double, double> get_limits(const unsigned int d);	// return limits of dimension d
	double get_span(const unsigned int d);	// return span of dimension d
	double get_volume();	// returns volume
	std::vector<double> get_x();	// returns x
	double get_x(const unsigned int pos);	// returns pos'th element of x
// PRINT FUNCTIONS
	void ret_x(std::ostream &out);	// writes vector to screen
// VECTOR FUNCTIONS
	void set_x(const std::vector<double> &new_x);	// sets x manually (eg in the beginning)
	void fill_x(const double val);	// fills x with copies of val
	double l2_norm_x();				// calculates l2-norm of x
};

// VECTOR OPERATORS AND FUNCTIONS
std::vector<double> operator*(const std::vector<double>& v, double alfa);
std::vector<double> operator*(double alfa, const std::vector<double>& v);
std::vector<double> operator+(const std::vector<double>& v1, const std::vector<double>& v2);
std::vector<double> operator-(const std::vector<double>& v1, const std::vector<double>& v2);
double l2_norm(const std::vector<double> &vec);	// calculates l2-norm of vector

#endif // !_mc_hpp_
