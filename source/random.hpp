#ifndef _random_hpp_
#define _random_hpp_

#include <random>
#include <chrono>		// seed

class random {
private:
	std::mt19937 gen;
	std::uniform_real_distribution<double> distribution;

public:
	random();
	random(const unsigned seed);
	double get();
};

#endif // !_random_hpp_
