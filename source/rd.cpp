#include "rd.hpp"

rd::rd(const unsigned int dim) : mc(dim) {
	std::fill(x.begin(), x.end(), 0.0);	// set start point to origin
}
rd::rd(const std::vector< std::pair<double, double> > &lims) : mc(lims) {
	std::fill(x.begin(), x.end(), 0.0);	// set start point to origin
}
void rd::update() {
	for (int i = 0; i < x.size(); i++) x.at(i) = get(i);
	step_nr++;
}
double rd::get(const unsigned int i) {
	std::pair<double, double> limit = limits.at(i);
	return limit.first+spans.at(i)*distribution(gen);
}