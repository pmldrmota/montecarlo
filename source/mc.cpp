#include "mc.hpp"

mc::mc(unsigned int dim) : dim(dim) {}
double mc::ret_rd() { 
	unsigned seed1 = std::chrono::system_clock::now().time_since_epoch().count();
	std::mt19937 gen(seed1);
	std::uniform_real_distribution<double> distribution(0, 1);
	auto dice = std::bind(distribution, gen); 
	return dice();
}