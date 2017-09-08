#include "mc.hpp"

mc::mc(const unsigned int dim) : dim(dim) {
	x.resize(dim);
}
unsigned int mc::dimension() { 
	return dim; 
}
void mc::ret_x(std::ostream &out) {
	if (x.empty()) return;
	out << "(";
	for (auto it : x) {
		out << it;
		if (it != x.back()) out << ", ";
	}
	out << ")" << std::endl;
}
std::vector<double> mc::get_x() { 
	return x; 
}
double mc::l2_norm(const std::vector<double> &vec) {
		double accum = 0.;
		for (double it : vec) {
			accum += it * it;
		}
		return std::sqrt(accum);
}
double mc::l2_norm_x() {
	double accum = 0.;
	for (double it : x) {
		accum += it * it;
	}
	return std::sqrt(accum);
}