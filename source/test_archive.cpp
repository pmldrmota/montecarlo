#include <iostream>
#include <fstream>
#include "rd.hpp"
#include "cereal\archives\binary.hpp"
#include <exception>


int main() {

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
	}
	return 0;
}