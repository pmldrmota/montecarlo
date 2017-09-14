#include "mcmc.hpp"

mcmc::mcmc(unsigned int dimension) : mc(dimension) {
	starting_point();
	y.resize(dim);
}
mcmc::mcmc(const std::vector< std::pair<double, double> > &lims) : mc(lims) {
	starting_point();
	y.resize(dim);
}
void mcmc::starting_point() {
	std::pair<double, double> limit;
	for (int i = 0; i < x.size(); i++) {
		limit = limits.at(i);
		x.at(i) = limit.first + spans.at(i)*uniform_dist(gen);
	}
	trace.push_back(x);
}
void mcmc::propose() {
	y = x;
}
void mcmc::update() {
	do {
		propose();
	} while (!y_inside_space());
	if (success()) x = y;
	step_nr++;
	trace.push_back(x);
}
bool mcmc::success() {
	return true;
}
bool mcmc::y_inside_space() {
	bool inside{ true };
	std::pair<double, double> limit;
	for (int i = 0; i < dim; i++) {
		limit = limits.at(i);
		if (y.at(i) < limit.first || y.at(i) > limit.second) {
			inside = false;
			break;
		}
	}
	return inside;
}