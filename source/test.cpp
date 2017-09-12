#include <iostream>
#include <vector>
#include <tuple>
#include "metrop.hpp"

int main() {
	std::vector<std::pair<double, double>> lims;
	lims.push_back(std::pair<double, double>(0, 10));

	metrop inst(lims);
	inst.set_proposal_width(2);

	inst.ret_x(std::cout);
	for (int i = 0; i < 10; i++) {
		inst.update();
		std::cout << inst.get_step_nr() << std::endl;
		inst.ret_x(std::cout);
		std::cout << std::endl;
	}
	return 0;
}