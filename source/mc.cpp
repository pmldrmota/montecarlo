#include "mc.hpp"

mc::mc(unsigned int dim) : dim(dim) {
	rdvec.resize(dim);
}
unsigned int mc::dimension() { return dim; }
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
	out << ")" << std::endl;
}
std::vector<double> mc::get_rdvec() { return rdvec; }
double mc::l2_norm(const std::vector<double> &vec) {
		double accum = 0.;
		for (double it : vec) {
			accum += it * it;
		}
		return std::sqrt(accum);
}
double mc::l2_norm_rdvec() {
	double accum = 0.;
	for (double it : rdvec) {
		accum += it * it;
	}
	return std::sqrt(accum);
}