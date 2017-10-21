#include "common\mc.hpp"

mc::mc(const unsigned dim) : step_nr(0) {
	for (int i = 0; i < dim; ++i) limits.push_back(std::pair<double, double>(0.0, 1.0));
	complement_space_vars();
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	gen.seed(seed);
	x.resize(dim);
}
mc::mc(const std::vector< std::pair<double, double> > &lims) : step_nr(0), limits(lims) {
	complement_space_vars();
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	gen.seed(seed);
	x.resize(dim);
}
mc::mc(mc_archive &ar) : limits(ar.limits), trace(ar.trace), x(ar.x), step_nr(ar.step_nr) {
	complement_space_vars();
	// restore random seed
	std::stringstream ss;
	ss.setf(std::ios_base::dec, std::ios_base::basefield);
	ss << ar.gen_status;
	ss >> gen;
}
void mc::complement_space_vars() {
	dim = limits.size();
	// set spans
	for (auto it : limits) spans.push_back(it.second - it.first);
	// set volume
	volume = 1.0;
	for (auto it : spans) volume *= it;
}

void mc::archivise() {
	std::ofstream os("backup.bin", std::ios::binary);
	cereal::BinaryOutputArchive oarchive(os); // Create an output archive
	oarchive(get_mc_archive());	// Archivate the mc_archive
}
mc_archive mc::get_mc_archive() {
	std::stringstream ss;
	ss.setf(std::ios_base::dec, std::ios_base::basefield);
	ss << gen;
	return mc_archive{ ss.str(), x, step_nr, limits, trace };
}

void mc::update() {
	make_step();	// happens somewhere else
	++step_nr;
	trace.push_back(x);
}
void mc::update(const std::vector<double> &new_x) {
	if (new_x.size() != dim) throw std::runtime_error("vector passed to update has wrong dimension");
	x = new_x;
	++step_nr;
	trace.push_back(x);
}
void mc::make_step() {
	return;
}
unsigned mc::dimension() { 
	return dim; 
}
unsigned mc::get_step_nr() {
	return step_nr;
}
std::pair<double, double> mc::get_limits(const unsigned d) {
	return limits.at(d);
}
double mc::get_span(const unsigned d) {
	return spans.at(d);
}
double mc::get_volume() {
	return volume;
}
std::vector<double> mc::get_x() { 
	return x; 
}
double mc::get_x(const unsigned pos) {
	return x.at(pos);
}
std::vector<std::vector<double>> mc::get_trace() {
	return trace;
}
std::vector<double> mc::get_trace(const unsigned i) {
	return trace.at(i);
}
void mc::print_x(std::ostream &out) {
	if (x.empty()) return;
	out << "(";
	const unsigned size{ x.size() };
	for (int i = 0; i < size; ++i) {
		out << x.at(i);
		if (i != size-1) out << ", ";
	}
	out << ")" << std::endl;
}
void mc::print_histogram(std::ostream &out, const unsigned n_bins, const unsigned var) {
	std::map<unsigned, unsigned> hist = histogram(n_bins, var);
	int max_count = std::max_element(hist.begin(), hist.end(), compare_counts)->second;
	double a{ limits[var].first };
	double span{ spans.at(var) };
	for (auto it : hist) {
		std::cout << std::fixed << std::setprecision(2) << a + it.first*span / n_bins << " - " << a + (it.first+1)*span / n_bins << "\t" << it.second << "\t";
		for (int i = 0; i < (1.0*it.second / max_count) * 25; ++i) std::cout << "X";
		std::cout << std::endl;
	}
}
double mc::l2_norm_x() {
	return l2_norm(x);
}
double mc::autocorrelation(const unsigned k) {
	std::vector<double> mean_trace(dim);
	for (int d = 0; d < dim; ++d) mean_trace.at(d) = expectation(d);
	double acsum{ 0.0 };
	for (int i = 0; i < step_nr - k; ++i) acsum += ((trace.at(i) - mean_trace)*(trace.at(i + k) - mean_trace))/step_nr;
	return acsum;
}
std::map<unsigned, unsigned> mc::histogram(const unsigned n_bins, const unsigned var) {
	std::map<unsigned, unsigned> hist;
	if (var > dim) std::cerr << "the variable passed to histogram is too large" << std::endl;
	else {
		std::vector<double> values;
		for (int i = 0; i < step_nr; ++i) {
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
double mc::expectation(const unsigned var) {
	double sum{ 0.0 };
	for (auto it : trace) sum += it.at(var);
	return sum / step_nr;
}
double mc::variance(const unsigned var) {
	double qsum{ 0.0 }, mu{ expectation(var) };
	for (auto it : trace) qsum += std::pow(it.at(var) - mu, 2);
	return qsum / step_nr;
}
void mc::reset() {
	trace.clear();
	step_nr = 0;
}
void mc::write_trace_to_file(std::ofstream &outf) {
	std::string line;
	for (auto point : trace) {					// iterate over all points in the trace
		for (auto coord : point) {
			line.append(std::to_string(coord));	// append coordinate
			line.append("\t");					// tabs between coordinates
		}
		line.pop_back();						// delete last tab

		outf << line << std::endl;				// write line with coordinates of one point to the file
		line.clear();
	}
}
void mc::write_histogram_to_file(std::ofstream &outf, const unsigned n_bins, const unsigned var) {
	std::map<unsigned, unsigned> hist = histogram(n_bins, var);	
	double a{ limits[var].first };
	double span{ spans.at(var) };
	for (auto it : hist) {
		outf << std::fixed << std::setprecision(3) << a + (2 * it.first - 1)*span / (2 * n_bins) << "\t" << it.second << std::endl;
	}
}
void mc::write_autocorrelation_to_file(std::ofstream &outf, const unsigned max_lag) {
	for (int k = 0; k < max_lag; k++) outf << autocorrelation(k) << std::endl;
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
	for (int i = 0; i < v1.size(); ++i) r += v1.at(i)*v2.at(i);
	return r;
}
std::vector<double> operator+(const std::vector<double>& v1, const std::vector<double>& v2)
{
	std::vector<double> y;
	if (v1.size() != v2.size()) {
		std::cerr << "vectors passed to + have different sizes." << std::endl;
		return y;
	}
	for (int i = 0; i < v1.size(); ++i) y.push_back(v1.at(i) + v2.at(i));
	return y;
}
std::vector<double> operator-(const std::vector<double>& v1, const std::vector<double>& v2)
{
	std::vector<double> y;
	if (v1.size() != v2.size()) {
		std::cerr << "vectors passed to - have different sizes." << std::endl;
		return y;
	}
	for (int i = 0; i < v1.size(); ++i) y.push_back(v1.at(i) - v2.at(i));
	return y;
}
double l2_norm(const std::vector<double> &vec) {
	double sq = std::inner_product(vec.begin(), vec.end(), vec.begin(), 0.0);
	return std::sqrt(sq);
}
double mean(const std::vector<double> &vec) {
	return std::accumulate(vec.begin(), vec.end(), 0.0)/vec.size();
}
bool compare_counts(const std::pair<unsigned, unsigned>&a, const std::pair<unsigned, unsigned>&b)
{
	return a.second<b.second;
}