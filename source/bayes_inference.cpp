#include <iostream>
#include <vector>
#include <tuple>
#include <map>
#include "inference.hpp"

std::vector<std::vector<double>> generate_samples(const int num_samples, const double mu = 0.0, const double sigma = 1.0) {
	std::mt19937 gen;
	std::normal_distribution<double> gaussverteilung(mu, sigma);
	std::vector<std::vector<double>> observations;
	std::vector<double> observation;
	for (int i = 0; i < num_samples; i++) {
		observation.push_back(gaussverteilung(gen));
		observations.push_back(observation);
		observation.pop_back();
	}
	return observations;
}

/*std::map<double, unsigned int> histogram(const unsigned int n_bins, const std::vector<std::vector<double>> &posteriors) {
	std::map<double, unsigned int> hist;
	for (auto it : posteriors) ++hist[];
}*/

int main() {
	int n;
	std::cout << "samples: ";
	std::cin >> n;

	int max;
	std::cout << "steps: ";
	std::cin >> max;

	std::vector<std::pair<double, double>> lims;
	lims.push_back(std::pair<double, double>(-10, 10));	// limits of variable 0 ( mu )
	lims.push_back(std::pair<double, double>(0.000001, 5));	// limits of variable 1 ( sigma )

	inference inst(lims, std::vector<double>{ 0.0, 1.0 });
	inst.set_proposal_width(1);

	std::vector<std::vector<double>> observations = generate_samples(n, 6.0, 0.5);

	for (int i = 0; i < max; i++) {
		inst.update(observations);
		inst.ret_x(std::cout);
	}
	// erstelle histogramm
	//std::map<double, unsigned int> hist = histogram(10, posteriors);

	return 0;
}