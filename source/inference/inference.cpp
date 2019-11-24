#include "inference\inference.hpp"

inference::inference(const unsigned int dim, log_model_distribution_PTR log_model_distribution) : mcmc(dim), log_model_distribution(log_model_distribution), proposal_width(1.0), dim_data(0) {
	param_attributes = std::vector<param_type>(dim, linear);
	prior_distributions = std::vector<std::tuple<dist_type, double, double>>(dim, std::tuple<dist_type, double, double>{uniform, 0, 1});
}
inference::inference(const std::vector< std::pair<double, double> > &lims, log_model_distribution_PTR log_model_distribution) : mcmc(lims), log_model_distribution(log_model_distribution), proposal_width(1.0), dim_data(0) {
	param_attributes = std::vector<param_type>(dim, linear);
	for (auto it : limits) {
		prior_distributions.push_back(std::tuple<dist_type, double, double>{uniform, it.first, it.second});
	}
}
inference::inference(inference_archive &ar, log_model_distribution_PTR log_model_distribution) : log_model_distribution(log_model_distribution), mcmc(ar.mcdata), observations(ar.observations), prior_distributions(ar.prior_distributions), param_attributes(ar.param_attributes), proposal_width(ar.proposal_width) {}

void inference::set_param_attributes(const std::vector<param_type> &types){
	if (types.size() != dim) std::cerr << "param types vector has wrong dimension" << std::endl;
	param_attributes = types;
}

void inference::archivise() {
	std::ofstream os("backup.bin", std::ios::binary);
	cereal::BinaryOutputArchive oarchive(os); // Create an output archive
	oarchive(get_inference_archive());	// Archivate the mc_archive
}
inference_archive inference::get_inference_archive() {
	return inference_archive{ get_mc_archive(), prior_distributions, observations, param_attributes, proposal_width };
}

void inference::set_prior_distributions(const std::vector<std::tuple<dist_type, double, double>> &pridist) {
	if (pridist.size() != dim) std::cerr << "prior distribution settings have wrong dimension" << std::endl;
	else prior_distributions = pridist;
}
double inference::get_std_normal_1d() {
	return gauss(gen);
}
void inference::propose() {
	for (int i = 0; i < dim; i++) {
		y.at(i) = x.at(i) + get_std_normal_1d()*proposal_width*spans.at(i) / 100;

		if(param_attributes.at(i) == periodic) {
			if (y.at(i) < limits.at(i).first || y.at(i) > limits.at(i).second) { // and if it is ouside the limits
				// wrap it around the limits (https://stackoverflow.com/questions/11980292/how-to-wrap-around-a-range)
				double y_norm = (y.at(i) - limits.at(i).first) / spans.at(i);
				double m_norm = std::fmod(y_norm, 1.0);
				if(m_norm < 0.0) m_norm += 1.0;
				y.at(i) = spans.at(i) * m_norm + limits.at(i).first;
			}
		}
	}
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

double inference::log_likelihood(const std::vector<double> &z) {
	double likeli{ 0.0 };
	for (auto obs : observations) {
		likeli += log_model_distribution(obs, z);
	}
	return likeli;
}
double inference::log_prior_distribution(const unsigned int d, const double wert) {
	std::tuple<dist_type, double, double> param{ prior_distributions.at(d) };

	switch (std::get<0>(param))
	{
	case uniform:
		// std::get<1>(param) = lower boundary, std::get<2>(param) = upper boundary
		if (wert < std::get<1>(param) || wert > std::get<2>(param)) return -200;	// extremely unlikely
		else return std::log(1/(std::get<2>(param) - std::get<1>(param)));	// uniform probability
		break;
	case normal:
		// std::get<1>(param) = mu, std::get<2>(param) = sigma
		return -std::pow((wert - std::get<1>(param)) / std::get<2>(param), 2) / 2 - 0.9189385332 - std::log(std::get<2>(param));
		break;
	case logistic:
		// std::get<1>(param) = mu, std::get<2>(param) = s
		return -2 * std::log(std::cosh((wert - std::get<1>(param)) / (2 * std::get<2>(param)))) - 1.3862943611 - std::log(std::get<2>(param));
	case exponential:
		// std::get<1>(param) = lambda
		if(wert >= 0) return -std::get<1>(param) * wert + std::log(std::get<1>(param));
		else return -200;
	case chi_squared:
		// std::get<1>(param) = n
		if (wert > 0) return -wert / 2 - std::get<1>(param)*0.3465735903 - std::lgamma(std::get<1>(param) / 2) + (std::get<1>(param) / 2 - 1)*std::log(wert);
		else return -200;
	case lorentz:
		// std::get<1>(param) = s, std::get<2>(param) = t
		return -1.1447298858 + std::log(std::get<1>(param)) - std::log(std::get<1>(param)*std::get<1>(param) + std::pow(wert - std::get<2>(param), 2));
	case poisson:
		// std::get<1>(param) = lambda
		return -std::get<1>(param) - std::lgamma(wert + 1) + wert*std::log(std::get<1>(param));
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
