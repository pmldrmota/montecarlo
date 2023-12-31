#include "rd\rd.hpp"
#include <iostream>
#include <tuple>
#include <vector>
#include <cmath>

double f(rd &X) {
	return X.get_x(0)*X.get_x(1)*X.get_x(1)*std::exp(X.get_x(2));	// integrand
}

int main() {
	std::vector<std::pair<double, double>> lims;
	lims.push_back(std::pair<double, double>(0, 2));
	lims.push_back(std::pair<double, double>(-1, 1));
	lims.push_back(std::pair<double, double>(0, 3));

	rd inst(lims);

	double eval, accum{ 0 }, sqares{ 0 };
	int max;
	std::cout << "Anzahl Punkte: ";
	std::cin >> max;
	std::cout << std::endl;

	for (int i = 0; i < max; i++) {
		inst.update();
		eval = f(inst);
		accum += eval;
		sqares += eval*eval;
	}
	double integral = inst.get_volume()*accum / max;
	double variance = (inst.get_volume()*inst.get_volume() * sqares / max - integral*integral) / max;
	std::cout << "Integral ~ " << integral << std::endl;
	std::cout << "Varianz ~ " << variance << std::endl;
	return 0;
}