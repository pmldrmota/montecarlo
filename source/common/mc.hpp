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
#include <sstream>		// gen_status

#include "archives\archives.hpp"

class mc {
protected:
// SPACE VARIABLES
	unsigned dim;		// dimension of space
	std::vector< std::pair<double, double> > limits;	// limits of the space
	std::vector<double> spans;	// span of the dimensions of the space
	double volume;	// volume of the space
	void complement_space_vars();	// fill spans and volume
// UPDATE
	virtual void make_step();		// defined in inherited class: update x
// ALGORITHM VARIABLES
	unsigned step_nr;	// number of steps already done
	std::vector<double> x;	// contains current position
	std::vector<std::vector<double>> trace;	// contains all positions since construction
// RANDOM GENERATOR VARIABLES
	std::mt19937 gen;		// random number generator needed for all distributions
// ARCHIVE
	mc_archive get_mc_archive();

public:
// CONSTRUCTOR
	mc(const unsigned dim);	// initialises limits [0,1] for all dimensions
	mc(const std::vector< std::pair<double, double> > &lims); // sets dimension to lims.size()
	mc(mc_archive &ar);	// re-constructs instance of mc from a cereal binary file
// UPDATE
	void update();	// calls virtual void make_step (defined in inherited class) and increases step_nr and pushes x back to trace
	//void update(void(*make_step_manually)(std::vector<double>&));	// calls the function passed through the argument, which receives access to x in order to change x
// RETURN VARIABLES
	unsigned dimension();		// returns dim
	unsigned get_step_nr();		// returns nr_steps
	std::pair<double, double> get_limits(const unsigned d);	// return limits of dimension d
	double get_span(const unsigned d);	// return span of dimension d
	double get_volume();	// returns volume
	std::vector<double> get_x();	// returns x
	double get_x(const unsigned pos);	// returns pos'th element of x
	std::vector<std::vector<double>> get_trace();	// returns trace
	std::vector<double> get_trace(const unsigned i);	// returns i'th position of the trace
// PRINT FUNCTIONS
	void print_x(std::ostream &out);	// writes vector to screen
	void print_histogram(std::ostream &out, const unsigned n_bins, const unsigned var);	// writes a histogram onto the console (bins from up to down, bars made up of XXXXXX)
// VECTOR FUNCTIONS
	double l2_norm_x();				// calculates l2-norm of x
// ARCHIVE
	virtual void archivise();
// DIAGNOSTIC AO FUNCTIONS
	double autocorrelation(const unsigned k);		// evaluates the empirical autocorrelation of the trace with lag k
	std::map<unsigned, unsigned> histogram(const unsigned n_bins, const unsigned var);	// returns a histogram with bins numerated from 0 == [a,a+span/n_bins] to n_bins == [b-span/n_bins,b]
	double expectation(const unsigned var);	// calculates expectation value of the distribution of var'th variable
	double variance(const unsigned var);	// calculates variance of the distribution of var'th variable
	void reset();	// resets trace and nr_steps, but keeps limits, current x and random seed
	void write_trace_to_file(std::ofstream &outf);	// writes the trace to a file (given by argument std::ofstream outf)
	void write_autocorrelation_to_file(std::ofstream &outf, const unsigned max_lag);	// autocorrelation(k) with k=0...max_lag to a file for subsequent plotting
};

// VECTOR OPERATORS AND FUNCTIONS
std::vector<double> operator*(const std::vector<double>& v, double alfa);
std::vector<double> operator*(double alfa, const std::vector<double>& v);
double operator*(const std::vector<double>& v1, const std::vector<double>& v2);
std::vector<double> operator+(const std::vector<double>& v1, const std::vector<double>& v2);
std::vector<double> operator-(const std::vector<double>& v1, const std::vector<double>& v2);
double l2_norm(const std::vector<double> &vec);	// calculates l2-norm of vector
double mean(const std::vector<double> &vec);	// calculates the algebraic mean of a vector
bool compare_counts(const std::pair<unsigned, unsigned>&a, const std::pair<unsigned, unsigned>&b);	// return a.second < b.second;

#endif // !_mc_hpp_
