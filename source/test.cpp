#include "mc.hpp"

int main() {
	mc inst(4);
	inst.set_rdvec();
	inst.ret_rdvec(std::cout);
	std::cout << inst.l2_norm_rdvec() << std::endl;

	return 0;
}