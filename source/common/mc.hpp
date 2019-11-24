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
// - statistical treatment of trace
//
// does not contain any update mechanism!
//
//----------
// Author: Peter Drmota, MSc
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
// INTERNAL ALGORITHMS
	double halfsample(const std::vector<double> &x, const std::pair<unsigned, unsigned> range) const;	// recursive function that returns the mode eventually
// RANDOM GENERATOR VARIABLES
	std::mt19937 gen;		// random number generator needed for all distributions
// ARCHIVE
	mc_archive get_mc_archive() const;

public:
// CONSTRUCTOR
	mc(const unsigned dim);	// initialises limits [0,1] for all dimensions
	mc(const std::vector< std::pair<double, double> > &lims); // sets dimension to lims.size()
	mc(mc_archive &ar);	// re-constructs instance of mc from a cereal binary file
// UPDATE
	void update(bool traceflag = true);	// calls virtual void make_step (defined in inherited class) and increases step_nr. if traceflag = true, it pushes x back to trace
	void update(const std::vector<double> &new_x, bool traceflag = true);	// manually set x, increase step_nr and push x back to trace if traceflag = true
// RETURN VARIABLES
	unsigned dimension() const;		// returns dim
	unsigned get_step_nr() const;		// returns nr_steps
	std::pair<double, double> get_limits(const unsigned d) const;	// return limits of dimension d
	double get_span(const unsigned d) const;	// return span of dimension d
	double get_volume() const;	// returns volume
	std::vector<double> get_x() const;	// returns x
	double get_x(const unsigned pos) const;	// returns pos'th element of x
	std::vector<std::vector<double>> get_trace() const;	// returns trace
	std::vector<double> get_trace(const unsigned i) const;	// returns i'th position of the trace
// PRINT FUNCTIONS
	void print_x(std::ostream &out) const;	// writes vector to screen
	void print_histogram(std::ostream &out, const unsigned n_bins, const unsigned var) const;	// writes a histogram onto the console (bins from up to down, bars made up of XXXXXX)
// VECTOR FUNCTIONS
	double l2_norm_x() const;				// calculates l2-norm of x
// ARCHIVE
	virtual void archivise();
// DIAGNOSTIC AO FUNCTIONS
	double autocorrelation(const unsigned k) const;		// evaluates the empirical autocorrelation of the trace with lag k
	std::map<unsigned, unsigned> histogram(const unsigned n_bins, const unsigned var) const;	// returns a histogram with bins numerated from 0 == [a,a+span/n_bins] to n_bins == [b-span/n_bins,b]
	std::map<std::pair<unsigned, unsigned>, unsigned> histogram(const std::pair<unsigned, unsigned> n_bins, const std::pair<unsigned, unsigned> vars) const;
	double expectation(const unsigned var) const;	// calculates expectation value of the distribution of var'th variable
	double variance(const unsigned var) const;	// calculates variance of the distribution of var'th variable
	double covariance(const unsigned var1, const unsigned var2) const; 	// calculates covariance of the distribution of var1'th and var2'th variables
	double mode(const unsigned var) const;	// calculates the marginal mode of the distribution of var'th variable
	double median(const unsigned var) const;	// calculates the marginal median of the distribution of var'th variable
	void reset();	// resets trace and nr_steps, but keeps limits, current x and random seed
	void write_trace_to_file(std::ofstream &outf) const;	// writes the trace to a file (given by argument std::ofstream outf)
	void write_histogram_to_file(std::ofstream &outf, const unsigned n_bins, const unsigned var);	// writes histogram data to a file
	void write_histogram_to_file(std::ofstream &outf, const std::pair<unsigned, unsigned> n_bins, const std::pair<unsigned, unsigned> vars);
	void write_autocorrelation_to_file(std::ofstream &outf, const unsigned max_lag) const;	// autocorrelation(k) with k=0...max_lag to a file for subsequent plotting
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
