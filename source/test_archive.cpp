#include <iostream>
#include <fstream>
#include "rd.hpp"
#include "cereal\archives\binary.hpp"
#include <exception>


int main() {
	{
		std::ofstream os("archive.bin", std::ios::binary); // any stream can be used
		cereal::BinaryOutputArchive oarchive(os); // Create an output archive
		mc_archive old_archive;
		rd inst(2);
		inst.update();
		inst.update();
		inst.update();
		inst.update();
		old_archive = inst.archivise();
		oarchive(old_archive); // Write the data to the archive
		inst.print_x(std::cout);
		inst.update();
		inst.print_x(std::cout);
		inst.update();
		inst.print_x(std::cout);
		inst.update();
		inst.print_x(std::cout);
	}
	{
		std::ifstream is("archive.bin", std::ios::binary); // any stream can be used
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
	}
	return 0;
}