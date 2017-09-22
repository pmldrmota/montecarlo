#include "mcmc.hpp"

mcmc::mcmc(unsigned int dimension) : mc(dimension) {
	starting_point();
	y.resize(dim);
}
mcmc::mcmc(const std::vector< std::pair<double, double> > &lims) : mc(lims) {
	starting_point();
	y.resize(dim);
}
/*mcmc::mcmc(archive &ar) : mc(ar) {
	y.resize(dim);
}*/
void mcmc::starting_point() {
	std::pair<double, double> limit;
	for (int i = 0; i < x.size(); i++) {
		limit = limits.at(i);
		x.at(i) = limit.first + spans.at(i)*uniform_dist(gen);
	}
	trace.push_back(x);
}
void mcmc::burn_in(const unsigned int period) {
	for (int i = 0; i < period; i++) {
		do {
			propose();	// happens somewhere else
		} while (!y_inside_space());
		set_log_p_success();	// happens somewhere else
		if (success()) x = y;
	}
}
void mcmc::make_step() {
	do {
		propose();	// happens somewhere else
	} while (!y_inside_space());
	set_log_p_success();	// happens somewhere else
	if (success()) x = y;
}
void mcmc::propose() {
	y = x;
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
void mcmc::set_log_p_success() {
	log_p_success = 0;
}
bool mcmc::success() {
	if (log_p_success >= 0 || (uniform_dist(gen) < std::exp(log_p_success))) return true;	// shortcut property: exp(log_p_succ) can never overflow, because it is only evaluated if log_p_success >= 0
	else return false;
}
