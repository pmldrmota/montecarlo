#include "rd.hpp"
#include "mc.hpp"
#include <iostream>

int main() {
	rd inst(2);
	int drinnen{ 0 }, max;
	std::cout << "Anzahl Punkte: ";
	std::cin >> max;
	std::cout << std::endl;
	for (int i = 0; i < max; i++) {
		inst.update();
		if (inst.l2_norm_x() < 1.0) drinnen++;
	}
	std::cout << "Pi ~ " << 4.0*drinnen / max << std::endl;
	return 0;
}