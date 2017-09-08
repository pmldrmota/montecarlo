#ifndef _mc_class_h_
#define _mc_class_h_

#include <iostream>
#include <random>
#include <chrono>		// seed
#include <vector>
#include <cmath>

class mc {
private:
	unsigned int dim;	// dimension of space
	std::mt19937 gen;
	std::uniform_real_distribution<double> distribution;
	std::vector<double> rdvec;	// contains random doubles between 0 and 1

public:
	mc(unsigned int dim);
	unsigned int dimension();			// returns dimension
	void ret_rdvec(std::ostream &out);	// writes vector to screen
	void update_rdvec();				// generates a new random vector
	std::vector<double> get_rdvec();	// returns random vector
	double l2_norm(const std::vector<double> &vec);	// calculates l2-norm of vector
	double l2_norm_rdvec();				// calculates l2-norm of rdvec
};

#endif // !_mc_class_h
