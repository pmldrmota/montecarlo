#include "common\mcmc.hpp"

mcmc::mcmc(unsigned int dimension) : mc(dimension) {
	starting_point();
	y.resize(dim);
}
mcmc::mcmc(const std::vector< std::pair<double, double> > &lims) : mc(lims) {
	starting_point();
	y.resize(dim);
}
mcmc::mcmc(mc_archive &ar) : mc(ar) {
	y.resize(dim);
}
void mcmc::starting_point() {
	std::pair<double, double> limit;
	for (int i = 0; i < x.size(); i++) {
		limit = limits.at(i);
		x.at(i) = limit.first + spans.at(i)*uniform_dist(gen);
	}
}
void mcmc::burn_in(const unsigned int period) {
	for (int i = 0; i < period; i++) make_step();
}
void mcmc::make_step() {
	propose();	// happens somewhere else
	if(!y_inside_space()) y_mirror_into_space();	// if y is not inside the space, mirror it at the limits until it is inside
	set_log_p_success();	// happens somewhere else
	if (success()) x = y;
}
void mcmc::propose() {
	y = x;
}
bool mcmc::y_inside_space() {
	bool inside{ true };
	for (int i = 0; i < dim; i++) {
		if (y.at(i) < limits.at(i).first || y.at(i) > limits.at(i).second) {
			inside = false;
			break;
		}
	}
	return inside;
}
void mcmc::y_mirror_into_space() {
	for (int i = 0; i < dim; i++) {	// iterate over all dimensions
		do{
			if (y.at(i) < limits.at(i).first) {	// value at the left outside
				y.at(i) = 2 * limits.at(i).first - y.at(i);
			}
			if (y.at(i) > limits.at(i).second) { // value at the right outside
				y.at(i) = 2 * limits.at(i).second - y.at(i);
			}
		} while (y.at(i) < limits.at(i).first || y.at(i) > limits.at(i).second);	// mirror as often as it is necessary
	}
}
void mcmc::set_log_p_success() {
	log_p_success = 0;
}
bool mcmc::success() {
	if (log_p_success >= 0 || (uniform_dist(gen) < std::exp(log_p_success))) return true;	// shortcut property: exp(log_p_succ) can never overflow, because it is only evaluated if log_p_success >= 0
	else return false;
}
