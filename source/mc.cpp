#include "mc.hpp"

mc::mc(unsigned int dim) : dim(dim) {
	rdvec.resize(dim);
	gen.seed(std::chrono::system_clock::now().time_since_epoch().count());
}
unsigned int mc::dimension() { return dim; }
void mc::update_rdvec() {	
	for (auto &it : rdvec) it = distribution(gen);
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