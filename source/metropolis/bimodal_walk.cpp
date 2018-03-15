#include <iostream>
#include <stdio.h>			// für Pipe
#include "Windows.h"
#include <string>
#include <vector>
#include <tuple>
#include "metropolis\metropolis.hpp"

//extern "C" FILE *_popen(const char *command, const char *mode);
//extern "C" FILE *_pclose(FILE *stream);

double log_target_distribution(const std::vector<double> &x) {
	return -0.2*x[0]*x[0] + std::log(0.3 + 0.7*0.0000000021*std::exp(4 * x[0]));
}
void plot(FILE *pipe, const std::string &filename) {
	std::string plotstring = "plot '" + filename + "' with lines title '" + filename + "'\n";
	const char* plotchars = &plotstring[0];
	fprintf(pipe, plotchars);
	fflush(pipe);
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

	{
		std::ofstream autoc("autocorrelation.dat");
		inst.write_autocorrelation_to_file(autoc, 200);
		autoc.close();
	}
	{
		std::string filename{ "points.dat" };
		std::ofstream pointf(filename);
		inst.write_trace_to_file(pointf);
		pointf.close();
	}
	{
		std::string filename{ "hist.dat" };
		std::ofstream histf(filename);
		inst.write_histogram_to_file(histf, bins, 0);
		histf.close();
	}
	{
		FILE *pipe = _popen("gnuplot", "w");
		plot(pipe, "points.dat");
		system("pause");
		_pclose(pipe);
	}
	{
		FILE *pipe = _popen("gnuplot", "w");
		plot(pipe, "hist.dat");
		system("pause");
		_pclose(pipe);
	}
	{
		FILE *pipe = _popen("gnuplot", "w");
		plot(pipe, "autocorrelation.dat");
		system("pause");
		_pclose(pipe);
	}
	return 0;
}