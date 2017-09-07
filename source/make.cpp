#include <iostream>
#include <fstream>
#include <string>
#include <Windows.h>

int main() {
	std::ifstream makefile1("makefile_obj");
	std::ifstream makefile2("makefile_exe");
	std::string line = "mkdir release\\obj";
	static char* command = &line[0];
	system(command);

	while (!makefile1.eof()) {
		std::getline(makefile1, line);
		command = &line[0];
		system(command);
	}
	while (!makefile2.eof()) {
		std::getline(makefile2, line);
		command = &line[0];
		system(command);
	}

	line = "rmdir /s /Q release\\obj";
	command = &line[0];
	system(command);
	return 0;
}