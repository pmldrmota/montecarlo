#include <iostream>
#include <vector>
#include <tuple>
#include "metropolis\metropolis.hpp"

double log_target_distribution(const double &x) {
	if (x >= -10 && x <= 24) return 0;
	else return -100;
	//return -0.2*x*x + std::log(0.3 + 0.7*0.0000000021*std::exp(4 * x));
}
int main() {

	int max;
	std::cout << "steps: ";
	std::cin >> max;

	std::vector<std::pair<double, double>> lims;
	lims.push_back(std::pair<double, double>(-12, 26));	// limits of variable 0

	metropolis inst(lims, log_target_distribution);
	inst.set_proposal_width(5);
	inst.burn_in(500);

	for (int i = 0; i < max; i++) inst.update();

	unsigned int bins{ 100 };
	std::cout << "Verteilung: " << std::endl;
	inst.print_histogram(std::cout, bins, 0);

	/*std::ofstream autoc("autocorrelation.dat");
	inst.write_autocorrelation_to_file(autoc, 500);
	autoc.close();

	std::ofstream pointf("points.dat");
	inst.write_trace_to_file(pointf);
	pointf.close();*/

	return 0;
}