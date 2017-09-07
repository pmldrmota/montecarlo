#ifndef _mc_class_h_
#define _mc_class_h_

#include <iostream>
#include <random>
#include <functional>	// std::bind
#include <chrono>		// seed
#include <vector>
#include <cmath>

class mc {
private:
	unsigned int dim;	// dimension of space
	std::vector<double> rdvec;	// contains random doubles between 0 and 1

public:
	mc(unsigned int dim);
	unsigned int dimension();
	void ret_rdvec(std::ostream &out);
	void set_rdvec();
	std::vector<double> get_rdvec();
	double l2_norm(const std::vector<double> &vec);
	double l2_norm_rdvec();
};

#endif // !_mc_class_h
