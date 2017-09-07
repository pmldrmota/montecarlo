#ifndef _mc_class_h_
#define _mc_class_h_

#include <iostream>
#include <random>
#include <functional>	// std::bind
#include <chrono>		// seed
#include <vector>

class mc {
private:
	unsigned int dim;	// dimension of space
	std::vector<double> rdvec;

public:
	mc(unsigned int dim);
	void ret_rdvec(std::ostream &out);
	void set_rdvec();
};

#endif // !_mc_class_h
