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
#include <cmath>

class mc {
protected:
	unsigned int dim;		// dimension of space
	std::vector<double> x;	// contains random doubles between 0 and 1

public:
	mc(const unsigned int dim);

	unsigned int dimension();		// returns dimension
	void ret_x(std::ostream &out);	// writes vector to screen

	std::vector<double> get_x();	// returns x
	double l2_norm(const std::vector<double> &vec);	// calculates l2-norm of vector
	double l2_norm_x();				// calculates l2-norm of x
};

#endif // !_mc_hpp_
