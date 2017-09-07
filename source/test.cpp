#include "mc.hpp"

int main() {
	mc inst(6);
	inst.set_rdvec();
	inst.ret_rdvec(std::cout);

	return 0;
}