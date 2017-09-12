#include "metrop.hpp"

metrop::metrop(unsigned int dimension) : mc(dimension), proposal_width(1.0) {
	starting_point();
	y.resize(dim);
}
metrop::metrop(const std::vector< std::pair<double, double> > &lims) : mc(lims), proposal_width(1.0) {
	starting_point();
	y.resize(dim);
}
void metrop::starting_point() {
	std::uniform_real_distribution<double> uniform_dist;
	std::pair<double, double> limit;
	for (int i = 0; i < x.size(); i++) {
		limit = limits.at(i);
		x.at(i) = limit.first + spans.at(i)*uniform_dist(gen);
	}
}
void metrop::set_proposal_width(const double w) {
	proposal_width = w;
}

double metrop::get_std_normal_1d() {
	return gauss(gen);
}
std::vector<double> metrop::get_normal(const std::vector<double> &mu, const double sigma) {
	std::vector<double> v(dim);
	for (int i = 0; i < dim; i++) v.at(i) = mu.at(i) + get_std_normal_1d()*sigma;
	return v;
}
double metrop::normal_distribution(const std::vector<double> &mu, const double sigma, const std::vector<double> &X) {
	std::vector<double> x_minus_mu{ X - mu };
	double x_minus_mu_sq = std::inner_product(x_minus_mu.begin(), x_minus_mu.end(), x_minus_mu.begin(), 0.0);
	return std::exp(-x_minus_mu_sq / (2 * sigma*sigma)) / (std::sqrt(2 * PI)*sigma);
}

void metrop::propose() {
	for (int i = 0; i < dim; i++) y.at(i) = x.at(i) + get_std_normal_1d()*proposal_width;	// same as y = get_normal(); but without copying 
}
void metrop::update() {
	propose();
	if (success()) {
		x = y;
	}
	step_nr++;
}
bool metrop::success() {
	if(y_inside_space()) return true;
	else return false;
}
bool metrop::y_inside_space() {
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