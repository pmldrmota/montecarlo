#include "random.hpp"

random::random() { 
	gen.seed(std::chrono::system_clock::now().time_since_epoch().count()); 
}
random::random(const unsigned seed1) {
	gen.seed(seed1);
}
double random::get() {
	return distribution(gen);
}