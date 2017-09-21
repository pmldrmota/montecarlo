/* ----------
// Monte Carlo Class
// ----------
//
// sets up the space with dimension dim and limits and provides basic functionality as
// - the current position in space
// - the number of steps already taken
// - a history of all previous positions
// - a histogram of the positions visited in a given dimension
// - a random number generator
// - autocorrelation function that computes the value of autocorrelation of the trace with user-defined lag
// - function that calculates the l2-norm of the current x
//
// does not contain any update mechanism!
//
//----------
// Author: Peter Drmota, BSc
// ----------
*/
#ifndef _mc_hpp_
#define _mc_hpp_
#ifndef __USE_MINGW_ANSI_STDIO
#define __USE_MINGW_ANSI_STDIO 0	// needed because of std::to_string
#endif // !__USE_MINGW_ANSI_STDIO

#include <iostream>		// print function
#include <fstream>		// write_trace_to_file
#include <string>		// write_trace_to_file
#include <iomanip>		// histogram
#include <vector>		// x, trace, limits
#include <map>			// histogram
#include <tuple>		// limits
#include <cmath>		// autocorrelation
#include <numeric>		// std::accumulate
#include <algorithm>	// std::max_element
#include <random>		// gen
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
	std::vector<std::vector<double>> trace;	// contains all positions since construction
// RANDOM GENERATOR VARIABLES
	std::mt19937 gen;		// random number generator needed for all distributions

public:
// CONSTRUCTOR
	mc(const unsigned int dim);	// initialises limits [0,1] for all dimensions
	mc(const std::vector< std::pair<double, double> > &lims); // sets dimension to lims.size()
// RETURN VARIABLES
	unsigned int dimension();		// returns dim
	unsigned int get_step_nr();		// returns nr_steps
	std::pair<double, double> get_limits(const unsigned int d);	// return limits of dimension d
	double get_span(const unsigned int d);	// return span of dimension d
	double get_volume();	// returns volume
	std::vector<double> get_x();	// returns x
	double get_x(const unsigned int pos);	// returns pos'th element of x
	std::vector<std::vector<double>> get_trace();	// returns trace
	std::vector<double> get_trace(const unsigned int i);	// returns i'th position of the trace
// PRINT FUNCTIONS
	void print_x(std::ostream &out);	// writes vector to screen
	void print_histogram(std::ostream &out, const unsigned int n_bins, const unsigned int var);
// VECTOR FUNCTIONS
	double l2_norm_x();				// calculates l2-norm of x
// DIAGNOSTIC AO FUNCTIONS
	double autocorrelation(const unsigned int k);		// evaluates the empirical autocorrelation of the trace with lag k
	std::map<unsigned int, unsigned int> histogram(const unsigned int n_bins, const unsigned int var);	// returns a histogram with bins numerated from 0 == [a,a+span/n_bins] to n_bins == [b-span/n_bins,b]
	double expectation(const unsigned int var);	// calculates expectation value of the distribution of var'th variable
	double variance(const unsigned int var);	// calculates variance of the distribution of var'th variable
	void reset();	// resets trace and nr_steps, but keeps limits, current x and random seed
	void write_trace_to_file(std::ofstream &outf);	// writes the trace to a file (given by argument std::ofstream outf)
	void write_autocorrelation_to_file(std::ofstream &outf, const unsigned int max_lag);	// writes k over autocorrelation(k) to a file for subsequent plotting
};

// VECTOR OPERATORS AND FUNCTIONS
std::vector<double> operator*(const std::vector<double>& v, double alfa);
std::vector<double> operator*(double alfa, const std::vector<double>& v);
double operator*(const std::vector<double>& v1, const std::vector<double>& v2);
std::vector<double> operator+(const std::vector<double>& v1, const std::vector<double>& v2);
std::vector<double> operator-(const std::vector<double>& v1, const std::vector<double>& v2);
double l2_norm(const std::vector<double> &vec);	// calculates l2-norm of vector
double mean(const std::vector<double> &vec);	// calculates the algebraic mean of a vector
bool compare_counts(const std::pair<unsigned int, unsigned int>&a, const std::pair<unsigned int, unsigned int>&b);	// return a.second < b.second;

#endif // !_mc_hpp_
