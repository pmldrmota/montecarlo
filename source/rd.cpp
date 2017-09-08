#include "rd.hpp"

rd::rd(const unsigned int dim) : mc(dim) {
	gen.seed(std::chrono::system_clock::now().time_since_epoch().count()); 
	update();
}
rd::rd(const unsigned int dim, const unsigned seed1) : mc(dim) {
	gen.seed(seed1);
	update();
}
void rd::update() {
	for (auto &it : x) it = get();
}
double rd::get() {
	return distribution(gen);
}