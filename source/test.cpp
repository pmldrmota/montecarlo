#include <iostream>
#include <iomanip>
#include <vector>
#include <tuple>
#include <map>
#include <algorithm> // std::max_element
#include "inference.hpp"

bool compare(const std::pair<unsigned int, unsigned int>&a, const std::pair<unsigned int, unsigned int>&b)
{
	return a.second<b.second;
}

std::map<unsigned int, unsigned int> histogram(const unsigned int n_bins, inference &inst, const unsigned int var) {
	std::vector<double> values, point;
	for (int i = 0; i < inst.get_step_nr(); i++) {
		point = inst.get_trace(i);
		values.push_back(point.at(var));
	}
	double a{ inst.get_limits(var).first };
	double span{ inst.get_span(var) };

	std::map<unsigned int, unsigned int> hist;
	int m;
	for (auto it : values) {
		m = n_bins*((it-a) / span);
		++hist[m];
	}
	return hist;
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

	std::mt19937 gen;
	std::normal_distribution<double> gaussverteilung(3, 1);
	std::uniform_real_distribution<double> gleichverteilung(-10, 10);
	std::exponential_distribution<double> exponentialverteilung(1);
	for (int i = 0; i < n; i++) {
		inst.add_observation(std::vector<double>{gaussverteilung(gen)});
	}

	for (int i = 0; i < max; i++) {
		inst.update();
	}

	// erstelle histogramm von mu
	std::cout << "Verteilung von Mu: " << std::endl;
	unsigned int von_dim{ 0 }, bins{ 20 };
	std::map<unsigned int, unsigned int> hist = histogram(bins, inst, von_dim);
	int max_count = std::max_element(hist.begin(), hist.end(), compare)->second;
	for (auto it : hist) {
		std::cout << std::fixed << std::setprecision(2) << inst.get_limits(von_dim).first + (2 * it.first + 1)*inst.get_span(von_dim) / (2 * bins) << "\t" << it.second << "\t";
		for (int i = 0; i < (1.0*it.second / max_count) * 10; i++) std::cout << "M";
		std::cout << std::endl;
	}
	// erstelle histogramm von sigma
	std::cout << std::endl << "Verteilung von Sigma: " << std::endl;
	von_dim = 1;
	bins = 20;
	hist = histogram(bins, inst, von_dim);
	max_count = std::max_element(hist.begin(), hist.end(), compare)->second;
	for (auto it : hist) {
		std::cout << std::fixed << std::setprecision(2) << inst.get_limits(von_dim).first + (2 * it.first + 1)*inst.get_span(von_dim) / (2 * bins) << "\t" << it.second << "\t";
		for (int i = 0; i < (1.0*it.second / max_count) * 10; i++) std::cout << "S";
		std::cout << std::endl;
	}
	return 0;
}