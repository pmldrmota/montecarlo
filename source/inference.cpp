#include "inference.hpp"

inference::inference(const unsigned int dim) : mcmc(dim), proposal_width(1.0) {
	prior.resize(dim);
}
inference::inference(const std::vector< std::pair<double, double> > &lims) : mcmc(lims), proposal_width(1.0) {
	prior.resize(dim);
}
inference::inference(const std::vector< std::pair<double, double> > &lims, const std::vector<double> &prior) : mcmc(lims), prior(prior), proposal_width(1.0) {}
void inference::set_prior(const std::vector<double> &v) {
	if (v.size() != dim) std::cerr << "prior has wrong dimension" << std::endl;
	else prior = v;
	return;
}

double inference::get_std_normal_1d() {
	return gauss(gen);
}
void inference::propose() {
	for (int i = 0; i < dim; i++) y.at(i) = x.at(i) + get_std_normal_1d()*proposal_width*spans.at(i) / 100;
}
void inference::set_proposal_width(const double w) {
	proposal_width = w;
}

double inference::neg_log_normal_distribution(const std::vector<double> &data, const std::vector<double> &X) {
	double mu{ X.at(0) }, wert{ data.at(0) };
	double sigma = X.at(1);
	return std::pow((wert - mu) / sigma, 2) / 2 + std::log(2 * PI) / 2 + std::log(sigma);
}
double inference::neg_log_likelihood(const std::vector<std::vector<double>> &observations, const std::vector<double> &z) {
	double likeli{ 0.0 };
	for (auto obs : observations) {
		likeli += neg_log_normal_distribution(obs, z);
	}
	return likeli;
}
double inference::neg_log_prior_distribution(const std::vector<double> &data) {
	double mu{ prior.at(0) }, wert{ data.at(0) };
	double sigma = prior.at(1);
	return std::pow((wert - mu) / sigma, 2) / 2 + std::log(2 * PI) / 2 + std::log(sigma);
}

bool inference::success(const std::vector<std::vector<double>> &observations) {
	double neg_log_likelihood_x{ neg_log_likelihood(observations,x) }, neg_log_likelihood_y{ neg_log_likelihood(observations,y) };
	double prior_x{ neg_log_prior_distribution(x) }, prior_y{ neg_log_prior_distribution(y) };
	double p_success = std::exp(prior_x - prior_y) * std::exp(neg_log_likelihood_x - neg_log_likelihood_y);
	if (uniform_dist(gen) < p_success) return true;
	else return false;
	return true;
}