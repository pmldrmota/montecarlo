#include <iostream>
#include <vector>
#include <tuple>
#include "metropolis\metropolis.hpp"

double log_target_distribution(const std::vector<double> &x) {
	double S{ 0.0 };
	double m{ 1.0 }, omega{ 1.0 };
	for (int i = 0; i < x.size() - 1; ++i) S += 0.5*m*std::pow(x[i + 1] - x[i], 2) + 0.5*m*std::pow(omega*x[i], 2);
	return (-1)*S;	// p_success = exp(-(S(y)-S(x))) -> log_p_success = S(x)-S(y) = log_target_distribution(y) - log_target_distribution(x) -> log_target_distribution(x) = -S(x)
}

int main() {

	int timesteps;
	std::cout << "timesteps: ";
	std::cin >> timesteps;
	int max;
	std::cout << "sweeps: ";
	std::cin >> max;
	
	std::vector<std::pair<double, double>> lims;
	for(int it=0; it<timesteps;++it) lims.push_back({ -10,10 });

	metropolis inst(lims, log_target_distribution);
	inst.set_proposal_width(5);
	inst.burn_in(100);

	for (int i = 0; i < max; i++) inst.update();

	{
		std::string filename{ "points.dat" };
		std::ofstream pointf(filename);
		inst.write_trace_to_file(pointf);
		pointf.close();
	}

	return 0;
}