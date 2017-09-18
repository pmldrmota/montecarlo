#include "bimodal.hpp"

bimodal::bimodal(const unsigned int dim) : mcmc(dim), proposal_width(1.0) {}
bimodal::bimodal(const std::vector< std::pair<double, double> > &lims) : mcmc(lims), proposal_width(1.0) {}

double bimodal::get_std_normal_1d() {
	return gauss(gen);
}
void bimodal::propose() {
	for (int i = 0; i < dim; i++) y.at(i) = x.at(i) + get_std_normal_1d()*proposal_width*spans.at(i) / 100;
}
void bimodal::set_proposal_width(const double w) {
	proposal_width = w;
}
double bimodal::log_target_distribution(const double x) {
	if (x >= -10 && x <= 24) return 0;
	else return -100;
	//return -0.2*x*x + std::log(0.3 + 0.7*0.0000000021*std::exp(4 * x));
}
void bimodal::set_log_p_success() {
	log_p_success = log_target_distribution(y.at(0)) - log_target_distribution(x.at(0));
}