#include <iostream>
#include <vector>
#include <tuple>
#include "bimodal.hpp"

int main() {

	int max;
	std::cout << "steps: ";
	std::cin >> max;

	std::vector<std::pair<double, double>> lims;
	lims.push_back(std::pair<double, double>(-50, 50));	// limits of variable 0

	bimodal inst(lims);
	inst.set_proposal_width(5);
	inst.burn_in(200);

	for (int i = 0; i < max; i++) inst.update();

	unsigned int bins{ 50 };
	std::cout << "Verteilung: " << std::endl;
	inst.print_histogram(std::cout, bins, 0);

	return 0;
}