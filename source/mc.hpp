#ifndef _mc_hpp_
#define _mc_hpp_

#include "random.hpp"
#include <iostream>
#include <vector>
#include <cmath>

enum chain_t {uncorrelated, metropolis_hastings, gibbs};

class mc {
private:
	unsigned int dim;		// dimension of space
	chain_t update_type;	// type of update for sampling (see enum chain_t for options)
	random random_number;	// class that produces random numbers
	std::vector<double> rdvec;	// contains random doubles between 0 and 1
	void update_rdvec(chain_t type);

public:
	mc(const unsigned int dim, const chain_t update_type);
	unsigned int dimension();			// returns dimension
	void ret_rdvec(std::ostream &out);	// writes vector to screen
	void update_rdvec();				// generates a new random vector
	std::vector<double> get_rdvec();	// returns random vector
	double l2_norm(const std::vector<double> &vec);	// calculates l2-norm of vector
	double l2_norm_rdvec();				// calculates l2-norm of rdvec
};

#endif // !_mc_hpp_
