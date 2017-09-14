#include <iostream>
#include <vector>
#include <tuple>
#include "inference.hpp"

void generate_observations(inference &inst, const int n, dist_type type, double a = 0.0, double b = 1.0) {
	std::mt19937 gen;
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

int main() {
	int n;
	std::cout << "samples: ";
	std::cin >> n;

	int max;
	std::cout << "steps: ";
	std::cin >> max;

	std::vector<std::pair<double, double>> lims;
	lims.push_back(std::pair<double, double>(-10, 10));	// limits of variable 0 ( mu )
	lims.push_back(std::pair<double, double>(0.000001, 10));	// limits of variable 1 ( sigma )

	inference inst(lims);
	inst.set_proposal_width(3);
	generate_observations(inst, n, normal, -4, 3);

	for (int i = 0; i < max; i++) inst.update();

	unsigned int bins{ 10 };
	std::cout << "Verteilung von Mu: " << std::endl;
	inst.print_histogram(std::cout, bins, 0);
	std::cout << std::endl << "Verteilung von Sigma: " << std::endl;
	inst.print_histogram(std::cout, bins, 1);

	return 0;
}