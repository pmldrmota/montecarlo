#include <iostream>
#include <vector>
#include <tuple>
#include <chrono>
#include "inference\inference.hpp"

void generate_observations(std::mt19937 &gen, inference &inst, const int n, dist_type type, double a = 0.0, double b = 1.0) {
	std::uniform_real_distribution<double> uniform_dist(a, b);
	std::normal_distribution<double> normal_dist(a, b);
	std::exponential_distribution<double> exp_dist(a);
	std::chi_squared_distribution<double> chi_sq_dist(a);
	std::cauchy_distribution<double> cauchy_dist(a, b);
	std::poisson_distribution<int> poisson_dist(a);

	switch (type)
	{
	case uniform:
		for (int i = 0; i < n; i++) inst.add_observation(std::vector<double>{uniform_dist(gen)});
		break;
	case normal:
		for (int i = 0; i < n; i++) inst.add_observation(std::vector<double>{normal_dist(gen)});
		break;
	case logistic:
		break;
	case exponential:
		for (int i = 0; i < n; i++) inst.add_observation(std::vector<double>{exp_dist(gen)});
		break;
	case chi_squared:
		for (int i = 0; i < n; i++) inst.add_observation(std::vector<double>{chi_sq_dist(gen)});
		break;
	case lorentz:
		for (int i = 0; i < n; i++) inst.add_observation(std::vector<double>{cauchy_dist(gen)});
		break;
	case poisson:
		for (int i = 0; i < n; i++) inst.add_observation(std::vector<double>{1.0*poisson_dist(gen)});
		break;
	default:
		break;
	}
}

double log_normal_distribution(const std::vector<double> &data, const std::vector<double> &X) {	// our model!!
	double mu{ X.at(0) }, wert{ data.at(0) };
	double sigma = X.at(1);
	return -std::pow((wert - mu) / sigma, 2) / 2 - 0.9189385332 - std::log(sigma);
}

int main() {
	int n;
	std::cout << "samples: ";
	std::cin >> n;

	int max;
	std::cout << "steps: ";
	std::cin >> max;

	std::mt19937 gen;
	gen.seed(std::chrono::system_clock::now().time_since_epoch().count());

	std::vector<std::pair<double, double>> lims;
	lims.push_back(std::pair<double, double>(-20, 20));	// limits of variable 0 ( mu )
	lims.push_back(std::pair<double, double>(-10, 20));	// limits of variable 1 ( sigma )

	std::vector<std::triple<dist_type, double, double>> parameter;
	parameter.push_back(std::triple<dist_type, double, double>(uniform, -10, 10));
	parameter.push_back(std::triple<dist_type, double, double>(uniform, 0, 10));

	inference inst(lims, log_normal_distribution);
	inst.set_prior_distributions(parameter);
	inst.set_proposal_width(3);

	unsigned int bins{ 15 };

	generate_observations(gen, inst, n, normal, -4, 3);
	for (int step = 0; step < max; ++step) inst.update();
	std::cout << std::endl << n << " samples: " << std::endl << std::endl;
		
	std::cout << "Verteilung von Mu: " << std::endl;
	std::cout << "Mittelwert: " << inst.expectation(0) << ", sqrt(Varianz): " << std::sqrt(inst.variance(0)) << std::endl;
	inst.print_histogram(std::cout, bins, 0);
	std::cout << std::endl << "Verteilung von Sigma: " << std::endl;
	std::cout << "Mittelwert: " << inst.expectation(1) << ", sqrt(Varianz): " << std::sqrt(inst.variance(1)) << std::endl;
	inst.print_histogram(std::cout, bins, 1);
	inst.reset();
	
	return 0;
}