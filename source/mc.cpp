#include "mc.hpp"

mc::mc(unsigned int dim) : dim(dim) {
	rdvec.resize(dim);
}
void mc::set_rdvec() { 
	unsigned seed1 = std::chrono::system_clock::now().time_since_epoch().count();
	std::mt19937 gen(seed1);
	std::uniform_real_distribution<double> distribution(0, 1);
	auto dice = std::bind(distribution, gen); 
	
	for (auto &it : rdvec) it = dice();
}
void mc::ret_rdvec(std::ostream &out) {
	if (rdvec.empty()) return;
	out << "(";
	for (auto it : rdvec) {
		out << it;
		if (it != rdvec.back()) out << ", ";
	}
	out << ")";
}