#include "metropolis\metropolis.hpp"

metropolis::metropolis(const unsigned int dim, log_target_distribution_PTR target_distribution) : mcmc(dim), proposal_width(1.0), log_target_distribution(target_distribution) {}
metropolis::metropolis(const std::vector< std::pair<double, double> > &lims, log_target_distribution_PTR target_distribution) : mcmc(lims), proposal_width(1.0), log_target_distribution(target_distribution) {}
metropolis::metropolis(metropolis_archive &ar, log_target_distribution_PTR target_distribution) : mcmc(ar.mcdata), proposal_width(ar.proposal_width), log_target_distribution(target_distribution) {}

void metropolis::archivise() {
	std::ofstream os("backup.bin", std::ios::binary);
	cereal::BinaryOutputArchive oarchive(os); // Create an output archive
	oarchive(get_metropolis_archive());	// Archivate the mc_archive
}
metropolis_archive metropolis::get_metropolis_archive() {
	return metropolis_archive{ get_mc_archive(), proposal_width };
}

double metropolis::get_std_normal_1d() {
	return gauss(gen);
}
void metropolis::propose() {
	for (int i = 0; i < dim; i++) y.at(i) = x.at(i) + get_std_normal_1d()*proposal_width*spans.at(i) / 100;
}
void metropolis::set_proposal_width(const double w) {
	proposal_width = w;
}
void metropolis::set_log_p_success() {
	log_p_success = log_target_distribution(y) - log_target_distribution(x);
} 