#include <iostream>
#include <fstream>
#include "metropolis\metropolis.hpp"
#include "cereal\archives\binary.hpp"
#include <exception>
#include <vector>
#include <tuple>

double log_target_distribution(const std::vector<double> &x) {
	return -0.2*x[0]*x[0] + std::log(0.3 + 0.7*0.0000000021*std::exp(4 * x[0]));
}

int main() {
	{
		std::vector<std::pair<double, double>> lims;
		lims.push_back(std::pair<double, double>(-20, 30));	// limits of variable 0

		metropolis inst(lims, log_target_distribution);
		inst.set_proposal_width(5);
		inst.burn_in(500);
		inst.update();
		inst.archivise();
		std::cout << "step_nr: " << inst.get_step_nr() << std::endl;
		inst.print_x(std::cout);
		inst.update();
		inst.print_x(std::cout);
		inst.update();
		inst.print_x(std::cout);
		inst.update();
		inst.print_x(std::cout);
	}
	{
		std::ifstream is("backup.bin", std::ios::binary);
		cereal::BinaryInputArchive iarchive(is); // Create an input archive
		metropolis_archive saved_archive;
		try {
			iarchive(saved_archive); // Read the data from the archive
		}
		catch (cereal::Exception ausnahme) {
			std::cerr << ausnahme.what() << std::endl;
			return 0;
		}
		metropolis inst2(saved_archive, log_target_distribution);

		std::cout << "step_nr: " << inst2.get_step_nr() << std::endl;
		inst2.print_x(std::cout);
		inst2.update();
		inst2.print_x(std::cout);
		inst2.update();
		inst2.print_x(std::cout);
		inst2.update();
		inst2.print_x(std::cout);
	}
/*	Teste rd class
		rd inst(2);
		inst.update();
		inst.update();
		inst.update();
		inst.update();
		
		inst.archivise();

		inst.print_x(std::cout);
		inst.update();
		inst.print_x(std::cout);
		inst.update();
		inst.print_x(std::cout);
		inst.update();
		inst.print_x(std::cout);

	{
		std::ifstream is("backup.bin", std::ios::binary);
		cereal::BinaryInputArchive iarchive(is); // Create an input archive
		mc_archive saved_archive;
		try {
			iarchive(saved_archive); // Read the data from the archive
		}
		catch (cereal::Exception ausnahme) {
			std::cerr << ausnahme.what() << std::endl;
			return 0;
		}
		rd inst2(saved_archive);
		std::cout << "step_nr: " << inst2.get_step_nr() << std::endl;
		inst2.print_x(std::cout);
		inst2.update();
		inst2.print_x(std::cout);
		inst2.update();
		inst2.print_x(std::cout);
		inst2.update();
		inst2.print_x(std::cout);
	}*/
	return 0;
}