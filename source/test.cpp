#include "mc.hpp"
#include <iostream>
#include "Windows.h"

int main() {
	mc inst(1);
	std::cout << inst.ret_rd() << std::endl;

	system("pause");
	return 0;
}