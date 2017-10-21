#include "metropolis\metropolis.hpp"
#include "rd\rd.hpp"
#include <cmath>

double log_target_distribution(const double &x) {
	return 0;
}
double volume_element(const std::vector<double> &x) {
	const unsigned N{ x.size()+1 };
	double sin_product{ 1 };
	for (int i = 1; i < N-1; ++i) sin_product *= std::pow(std::sin(x.at(i)), N - i - 1);
	return std::pow(x.at(0),N-1)*sin_product;
}

int main() {
	std::cout << "Dimension (greater than 1): ";
	unsigned dim;
	std::cin >> dim;
	std::cout << "Volume of " << dim << "D-Sphere: " << std::pow(PI, dim / 2.0) / std::tgamma(dim / 2.0 + 1) << std::endl;
	{
		metropolis walk(dim, log_target_distribution);
		walk.set_proposal_width(10);
		std::cout << "Number of Points for Metropolis Algorithm: ";
		unsigned N;
		std::cin >> N;
		unsigned count{ 0 };
		for (int i = 0; i < N; ++i) {
			walk.update(false);
			if (walk.l2_norm_x() <= 1) ++count;
		}
		std::cout << "Volume of " << dim << "D-Sphere: " << 1.0*std::pow(2,dim)*count / N << std::endl;
	}
	{
		rd walk(dim);
		std::cout << "Number of Points for Random Algorithm: ";
		unsigned N;
		std::cin >> N;
		unsigned count{ 0 };
		for (int i = 0; i < N; ++i) {
			walk.update(false);
			if (walk.l2_norm_x() <= 1) ++count;
		}
		std::cout << "Volume of " << dim << "D-Sphere: " << 1.0*std::pow(2, dim)*count / N << std::endl;
	}
	{
		std::vector<std::pair<double, double>> limits;
		limits.push_back(std::pair<double, double>{0, 1});
		for (int i = 0; i < dim-2; ++i) limits.push_back(std::pair<double, double>{0, PI});
		metropolis spherical(limits, log_target_distribution);
		spherical.set_proposal_width(10);
		std::cout << "Number of Points for Metropolis Algorithm in Spherical Coordinates: ";
		unsigned N;
		std::cin >> N;
		double mean_f{ 0 };
		for (int i = 0; i < N; ++i) {
			spherical.update(false);
			mean_f += volume_element(spherical.get_x());
		}
		mean_f /= N;
		std::cout << "Volume of " << dim << "D-Sphere: " << 2*PI*spherical.get_volume()*mean_f << std::endl;
	}
	{
		std::vector<std::pair<double, double>> limits;
		limits.push_back(std::pair<double, double>{0, 1});
		for (int i = 0; i < dim - 2; ++i) limits.push_back(std::pair<double, double>{0, PI});
		rd spherical(limits);
		std::cout << "Number of Points for Random Algorithm in Spherical Coordinates: ";
		unsigned N;
		std::cin >> N;
		double mean_f{ 0 };
		for (int i = 0; i < N; ++i) {
			spherical.update(false);
			mean_f += volume_element(spherical.get_x());
		}
		mean_f /= N;
		std::cout << "Volume of 3D-Sphere: " << 2 * PI*spherical.get_volume()*mean_f << std::endl;
	}
}