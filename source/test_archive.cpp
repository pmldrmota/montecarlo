#include <iostream>
#include <fstream>
#include "mc.hpp"
#include "cereal\archives\binary.hpp"
#include <exception>


int main() {
	{
		std::ofstream os("archive.bin", std::ios::binary); // any stream can be used
		cereal::BinaryOutputArchive oarchive(os); // Create an output archive

		mc inst(3);
		oarchive(inst.archivise()); // Write the data to the archive
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

		mc inst2(saved_archive);
		std::cout << inst2.dimension() << std::endl;
	}
	return 0;
}