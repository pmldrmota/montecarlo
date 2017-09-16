#include "inference.hpp"

inference::inference(const unsigned int dim) : mcmc(dim), proposal_width(1.0), prior_dist_type(uniform) {}
inference::inference(const std::vector< std::pair<double, double> > &lims) : mcmc(lims), proposal_width(1.0), prior_dist_type(uniform) {}

double inference::get_std_normal_1d() {
	return gauss(gen);
}
void inference::propose() {
	for (int i = 0; i < dim; i++) y.at(i) = x.at(i) + get_std_normal_1d()*proposal_width*spans.at(i) / 100;
}
void inference::set_proposal_width(const double w) {
	proposal_width = w;
}
void inference::set_observations(const std::vector<std::vector<double>> &obser) {
	observations = obser;
}
void inference::add_observation(const std::vector<double> &observation) {
	observations.push_back(observation);
}

double inference::neg_log_normal_distribution(const std::vector<double> &data, const std::vector<double> &X) {
	double mu{ X.at(0) }, wert{ data.at(0) };
	double sigma = X.at(1);
	return std::pow((wert - mu) / sigma, 2) / 2 + std::log(2 * PI) / 2 + std::log(sigma);
}
double inference::neg_log_likelihood(const std::vector<double> &z) {
	double likeli{ 0.0 };
	for (auto obs : observations) {
		likeli += neg_log_normal_distribution(obs, z);
	}
	return likeli;
}
double inference::neg_log_prior_distribution(const std::vector<double> &data) {
	switch (prior_dist_type)
	{
	case uniform: 
		return 0;
		break;
	case normal:	// brauche für jede Komponente des Datensatzes einen eigenen Satz Parameter der Verteilung: std::vector< std::pair<double mu, double sigma> > von dimension dim
		double mu{ 0.0 }, wert{ data.at(0) };
		double sigma{ 1.0 };
		return std::pow((wert - mu) / sigma, 2) / 2 + std::log(2 * PI) / 2 + std::log(sigma);
		break;
	}
}

bool inference::success() {
	double neg_log_likelihood_x{ neg_log_likelihood(x) }, neg_log_likelihood_y{ neg_log_likelihood(y) };
	double prior_x{ neg_log_prior_distribution(x) }, prior_y{ neg_log_prior_distribution(y) };
	double log_p_success = prior_x - prior_y + neg_log_likelihood_x - neg_log_likelihood_y;
	if (log_p_success >= 0 || (uniform_dist(gen) < std::exp(log_p_success))) return true;	// shortcut property: exp(log_p_succ) can never overflow, because it is only evaluated if log_p_success >= 0
	else return false;
}