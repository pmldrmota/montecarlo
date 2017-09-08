#include "mc.hpp"

mc::mc(const unsigned int dim, const chain_t update_type) : dim(dim), update_type(update_type) {
	rdvec.resize(dim);
	update_rdvec(uncorrelated);	// starting point
}
unsigned int mc::dimension() { 
	return dim; 
}
void mc::update_rdvec() {
	update_rdvec(update_type);
}
void mc::update_rdvec(chain_t type) {
	switch (type)
	{
	case uncorrelated:
		for (auto &it : rdvec) it = random_number.get();
		break;
	case metropolis_hastings:
		break;
	case gibbs:
		break;
	}
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
std::vector<double> mc::get_rdvec() { 
	return rdvec; 
}
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