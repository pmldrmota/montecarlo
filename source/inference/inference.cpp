#include "inference\inference.hpp"

inference::inference(const unsigned int dim) : mcmc(dim), proposal_width(1.0), dim_data(0) {
	prior_distributions = std::vector<std::triple<dist_type, double, double>>(dim, std::triple<dist_type, double, double>{uniform, 0, 1});
}
inference::inference(const std::vector< std::pair<double, double> > &lims) : mcmc(lims), proposal_width(1.0), dim_data(0) {
	for (auto it : limits) {
		prior_distributions.push_back(std::triple<dist_type, double, double>{uniform, it.first, it.second});
	}
}
inference::inference(inference_archive &ar) : mcmc(ar.mcdata), observations(ar.observations), prior_distributions(ar.prior_distributions), proposal_width(ar.proposal_width) {}

void inference::archivise() {
	std::ofstream os("backup.bin", std::ios::binary);
	cereal::BinaryOutputArchive oarchive(os); // Create an output archive
	oarchive(get_inference_archive());	// Archivate the mc_archive
}
inference_archive inference::get_inference_archive() {
	return inference_archive{ get_mc_archive(), prior_distributions, observations, proposal_width };
}

void inference::set_prior_distributions(const std::vector<std::triple<dist_type, double, double>> &pridist) {
	if (pridist.size() != dim) std::cerr << "prior distribution settings have wrong dimension" << std::endl;
	else prior_distributions = pridist;
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
void inference::set_observations(const std::vector<std::vector<double>> &obser) {
	unsigned int ddim{ obser[0].size() };
	for (auto it : obser) {
		if (ddim != it.size()) {
			std::cerr << "dimensions of observations are not equal" << std::endl;
			return;
		}
	}
	observations = obser;
	dim_data = ddim;
}
void inference::add_observation(const std::vector<double> &observation) {
	if (dim_data == 0) {
		observations.push_back(observation);
		dim_data = observation.size();
	}
	else
	{
		if (observation.size() != dim_data) {
			std::cerr << "dimension of observation is wrong" << std::endl;
			return;
		}
		else observations.push_back(observation);
	}
}
double inference::log_normal_distribution(const std::vector<double> &data, const std::vector<double> &X) {
	double mu{ X.at(0) }, wert{ data.at(0) };
	double sigma = X.at(1);
	return -std::pow((wert - mu) / sigma, 2) / 2 - 0.9189385332 - std::log(sigma);
}
double inference::log_likelihood(const std::vector<double> &z) {
	double likeli{ 0.0 };
	for (auto obs : observations) {
		likeli += log_normal_distribution(obs, z);
	}
	return likeli;
}
double inference::log_prior_distribution(const unsigned int d, const double wert) {
	std::triple<dist_type, double, double> param{ prior_distributions.at(d) };

	switch (param.first)
	{
	case uniform: 
		// param.second = lower boundary, param.third = upper boundary
		if (wert < param.second || wert > param.third) return -200;	// extremely unlikely
		else return std::log(1/(param.third - param.second));	// uniform probability
		break;
	case normal:
		// param.second = mu, param.third = sigma
		return -std::pow((wert - param.second) / param.third, 2) / 2 - 0.9189385332 - std::log(param.third);
		break;
	case logistic:
		// param.second = mu, param.third = s
		return -2 * std::log(std::cosh((wert - param.second) / (2 * param.third))) - 1.3862943611 - std::log(param.third);
	case exponential:
		// param.second = lambda
		if(wert >= 0) return -param.second * wert + std::log(param.second);
		else return -200;
	case chi_squared:
		// param.second = n
		if (wert > 0) return -wert / 2 - param.second*0.3465735903 - std::lgamma(param.second / 2) + (param.second / 2 - 1)*std::log(wert);
		else return -200;
	case lorentz:
		// param.second = s, param.third = t
		return -1.1447298858 + std::log(param.second) - std::log(param.second*param.second + std::pow(wert - param.third, 2));
	case poisson:
		// param.second = lambda
		return -param.second - std::lgamma(wert + 1) + wert*std::log(param.second);
	}
}
double inference::log_prior_p(const std::vector<double> &z) {
	double sum{ 0.0 };
	for (int i = 0; i < dim; i++) sum += log_prior_distribution(i, z.at(i));
	return sum;
}
void inference::set_log_p_success() {
	double log_likelihood_x{ log_likelihood(x) }, log_likelihood_y{ log_likelihood(y) };
	double prior_x{ log_prior_p(x) }, prior_y{ log_prior_p(y) };
	log_p_success = prior_y - prior_x + log_likelihood_y - log_likelihood_x;
}