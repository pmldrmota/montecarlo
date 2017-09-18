#include "mc.hpp"

mc::mc(const unsigned int dim) : dim(dim), step_nr(0) {
	gen.seed(std::chrono::system_clock::now().time_since_epoch().count());
	x.resize(dim);
	for(int i = 0; i < dim; i++) limits.push_back(std::pair<double, double>(0.0, 1.0));
	complement_space_vars();
}
mc::mc(const std::vector< std::pair<double, double> > &lims) : step_nr(0), limits(lims) {
	dim = limits.size();
	gen.seed(std::chrono::system_clock::now().time_since_epoch().count());
	x.resize(dim);
	complement_space_vars();
}
void mc::complement_space_vars() {
	// set spans
	for (auto it : limits) spans.push_back(it.second - it.first);
	// set volume
	volume = 1.0;
	for (auto it : spans) volume *= it;
}

unsigned int mc::dimension() { 
	return dim; 
}
unsigned int mc::get_step_nr() {
	return step_nr;
}
std::pair<double, double> mc::get_limits(const unsigned int d) {
	return limits.at(d);
}
double mc::get_span(const unsigned int d) {
	return spans.at(d);
}
double mc::get_volume() {
	return volume;
}
std::vector<double> mc::get_x() { 
	return x; 
}
double mc::get_x(const unsigned int pos) {
	return x.at(pos);
}
std::vector<std::vector<double>> mc::get_trace() {
	return trace;
}
std::vector<double> mc::get_trace(const unsigned int i) {
	return trace.at(i);
}
void mc::print_x(std::ostream &out) {
	if (x.empty()) return;
	out << "(";
	for (auto it : x) {
		out << it;
		if (it != x.back()) out << ", ";
	}
	out << ")" << std::endl;
}
void mc::print_histogram(std::ostream &out, const unsigned int n_bins, const unsigned int var) {
	std::map<unsigned int, unsigned int> hist = histogram(n_bins, var);
	int max_count = std::max_element(hist.begin(), hist.end(), compare_counts)->second;
	double a{ limits[var].first };
	double span{ spans.at(var) };
	for (auto it : hist) {
		std::cout << std::fixed << std::setprecision(2) << a + it.first*span / n_bins << " - " << a + (it.first+1)*span / n_bins << "\t" << it.second << "\t";
		for (int i = 0; i < (1.0*it.second / max_count) * 25; i++) std::cout << "X";
		std::cout << std::endl;
	}

}
double mc::l2_norm_x() {
	return l2_norm(x);
}
double mc::autocorrelation(const unsigned int k) {
	std::vector<double> mean_trace(dim);
	for (int i = 0; i < dim; i++) mean_trace.at(i) = mean(trace.at(i));
	double sum{ 0.0 };
	for (int i = 0; i < step_nr - k; i++) sum += ((trace.at(i) - mean_trace)*(trace.at(i + k) - mean_trace))/step_nr;
	return sum;
}
std::map<unsigned int, unsigned int> mc::histogram(const unsigned int n_bins, const unsigned int var) {
	std::map<unsigned int, unsigned int> hist;
	if (var > dim) std::cerr << "the variable passed to histogram is too large" << std::endl;
	else {
		std::vector<double> values;
		for (int i = 0; i < step_nr; i++) {
			values.push_back(trace[i].at(var));
		}
		double a{ limits[var].first };
		double span{ spans.at(var) };

		int m;
		for (auto it : values) {
			m = n_bins*((it - a) / span);
			++hist[m];
		}
	}
	return hist;
}
double mc::expectation(const unsigned int var) {
	double sum{ 0.0 };
	for (auto it : trace) sum += it.at(var);
	return sum / step_nr;
}
double mc::variance(const unsigned int var) {
	double qsum{ 0.0 }, mu{ expectation(var) };
	for (auto it : trace) qsum += std::pow(it.at(var) - mu, 2);
	return qsum / step_nr;
}
void mc::reset() {
	trace.clear();
	step_nr = 0;
}


std::vector<double> operator*(const std::vector<double>& v, double alfa)
{
	std::vector<double> y;
	int i{ 0 };
	for (auto it : v) y.push_back(alfa*it);
	return y;
}
std::vector<double> operator*(double alfa, const std::vector<double>& v)
{
	return v*alfa;
}
double operator*(const std::vector<double>& v1, const std::vector<double>& v2)
{
	double r{ 0 };
	if (v1.size() != v2.size()) {
		std::cerr << "vectors passed to scalar product * have different sizes." << std::endl;
		return 0;
	}
	for (int i = 0; i < v1.size(); i++) r += v1.at(i)*v2.at(i);
	return r;
}
std::vector<double> operator+(const std::vector<double>& v1, const std::vector<double>& v2)
{
	std::vector<double> y;
	if (v1.size() != v2.size()) {
		std::cerr << "vectors passed to + have different sizes." << std::endl;
		return y;
	}
	for (int i = 0; i < v1.size(); i++) y.push_back(v1.at(i) + v2.at(i));
	return y;
}
std::vector<double> operator-(const std::vector<double>& v1, const std::vector<double>& v2)
{
	std::vector<double> y;
	if (v1.size() != v2.size()) {
		std::cerr << "vectors passed to - have different sizes." << std::endl;
		return y;
	}
	for (int i = 0; i < v1.size(); i++) y.push_back(v1.at(i) - v2.at(i));
	return y;
}
double l2_norm(const std::vector<double> &vec) {
	double sq = std::inner_product(vec.begin(), vec.end(), vec.begin(), 0.0);
	return std::sqrt(sq);
}
double mean(const std::vector<double> &vec) {
	return std::accumulate(vec.begin(), vec.end(), 0.0)/vec.size();
}
bool compare_counts(const std::pair<unsigned int, unsigned int>&a, const std::pair<unsigned int, unsigned int>&b)
{
	return a.second<b.second;
}