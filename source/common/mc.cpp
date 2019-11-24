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
mc_archive mc::get_mc_archive() const {
	std::stringstream ss;
	ss.setf(std::ios_base::dec, std::ios_base::basefield);
	ss << gen;
	return mc_archive{ ss.str(), x, step_nr, limits, trace };
}

void mc::update(bool traceflag) {
	make_step();	// happens somewhere else
	++step_nr;
	if(traceflag) trace.push_back(x);
}
void mc::update(const std::vector<double> &new_x, bool traceflag) {
	if (new_x.size() != dim) throw std::runtime_error("vector passed to update has wrong dimension");
	x = new_x;
	++step_nr;
	if(traceflag) trace.push_back(x);
}
void mc::make_step() {
	return;
}
unsigned mc::dimension() const {
	return dim;
}
unsigned mc::get_step_nr() const {
	return step_nr;
}
std::pair<double, double> mc::get_limits(const unsigned d) const {
	return limits.at(d);
}
double mc::get_span(const unsigned d) const {
	return spans.at(d);
}
double mc::get_volume() const {
	return volume;
}
std::vector<double> mc::get_x() const {
	return x;
}
double mc::get_x(const unsigned pos) const {
	return x.at(pos);
}
std::vector<std::vector<double>> mc::get_trace() const {
	return trace;
}
std::vector<double> mc::get_trace(const unsigned i) const {
	return trace.at(i);
}
void mc::print_x(std::ostream &out) const {
	if (x.empty()) return;
	out << "(";
	const unsigned size{ static_cast<unsigned>(x.size()) };
	for (int i = 0; i < size; ++i) {
		out << x.at(i);
		if (i != size-1) out << ", ";
	}
	out << ")" << std::endl;
}
void mc::print_histogram(std::ostream &out, const unsigned n_bins, const unsigned var) const {
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
double mc::l2_norm_x() const {
	return l2_norm(x);
}
double mc::autocorrelation(const unsigned k) const {
	std::vector<double> mean_trace(dim);
	for (int d = 0; d < dim; ++d) mean_trace.at(d) = expectation(d);
	double acsum{ 0.0 };
	for (int i = 0; i < step_nr - k; ++i) acsum += ((trace.at(i) - mean_trace)*(trace.at(i + k) - mean_trace))/step_nr;
	return acsum;
}
std::map<unsigned, unsigned> mc::histogram(const unsigned n_bins, const unsigned var) const {
	std::map<unsigned, unsigned> hist;
	if (var > dim) std::cerr << "the variable passed to histogram is too large" << std::endl;
	else {
		std::vector<double> values;
		for (int i = 0; i < step_nr; ++i) {
			values.push_back(trace[i].at(var));
		}
		double a{ limits[var].first };
		double span{ spans.at(var) };

		for (auto it : values) {
			int m = n_bins*((it - a) / span);
			++hist[m];
		}
	}
	return hist;
}
std::map<std::pair<unsigned, unsigned>, unsigned> mc::histogram(const std::pair<unsigned, unsigned> n_bins, const std::pair<unsigned, unsigned> vars) const {
	std::map<std::pair<unsigned, unsigned>, unsigned> hist;
	if (vars.first > dim || vars.second > dim) std::cerr << "the variable passed to histogram is too large" << std::endl;
	else {
		std::vector<std::pair<double, double>> values;
		for (auto point : trace) values.push_back({point.at(vars.first), point.at(vars.second)});

		std::pair<double, double> a{ limits[vars.first].first, limits[vars.second].first };
		std::pair<double, double> span{ spans.at(vars.first), spans.at(vars.second) };

		for (auto p : values) {
			std::pair<unsigned, unsigned> m;
			m.first = n_bins.first*((p.first - a.first) / span.first);
			m.second = n_bins.second*((p.second - a.second) / span.second);
			++hist[m];
		}
	}
	return hist;
}
double mc::expectation(const unsigned var) const {
	double sum{ 0.0 };
	for (auto it : trace) sum += it.at(var);
	return sum / step_nr;
}
double mc::variance(const unsigned var) const {
	// calculate the standard deviation of the data
	double diff_sum{ 0. }, diff_sq_sum { 0. }, mean { expectation(var) };
	for (auto x : trace) {
		double diff{ x.at(var) - mean };
		diff_sum += diff;	// sum differences to the mean
		diff_sq_sum += diff*diff;	// sum squared differences
	}
	return (diff_sq_sum - diff_sum*diff_sum / step_nr) / (step_nr - 1);	// use corrected two-pass algorithm
}
double mc::covariance(const unsigned var1, const unsigned var2) const {
	double sum{ 0 }, mean1{ expectation(var1) }, mean2{ expectation(var2) };
	for (auto it : trace) sum += (it.at(var1) - mean1)*(it.at(var2) - mean2);
	return sum / (step_nr - 1);
}

// this function returns the half-sample mode within a range of indices in a SORTED vector x
// it calls itself recursively
double mc::halfsample(const std::vector<double> &x, const std::pair<unsigned, unsigned> range) const {
	const unsigned span{ range.second - range.first + 1 };
	// 3 trivial cases that return the estimate of the mode straight away
	if (span == 1) return x[range.first];
	if (span == 2) return (x[range.second] + x[range.first]) / 2;
	if (span == 3) {
		if (x[range.first + 1] - x[range.first] == x[range.first + 2] - x[range.first + 1]) return x[range.first + 1];
		else if (x[range.first + 1] - x[range.first] < x[range.first + 2] - x[range.first + 1]) return (x[range.first] + x[range.first + 1]) / 2;
		else return (x[range.first + 1] + x[range.first + 2]) / 2;
	}
	// non-trivial case that calls this function recursively
	else {
		double min_width{ x[range.second] - x[range.first] };	// we are looking for the index-RANGE of N/2 points which SPANs the smallest interval between values
		unsigned new_span{ (span + 1) / 2 }, new_range_first{ range.first };	// range -> indices, span -> difference of largest and smallest range-index
		for (int i = range.first; i <= range.second - new_span + 1; ++i) {	// move a window of new_span values from the smallest range index to higher indices to find
			double width{ x[i + new_span - 1] - x[i] };	// the smallest distance between the last and the first values of this window
			if (width < min_width) {	// remember the index i at which the range [i; i+new_span] produces the smallest difference in values between those limits
				min_width = width;
				new_range_first = i;
			}
		}
		return halfsample(x, { new_range_first, new_range_first + new_span - 1 });	// recursive function call with new range
	}
}

double mc::mode(const unsigned var) const {
	std::vector<double> sorted_data;
	for (auto it : trace) sorted_data.push_back(it.at(var));
	std::sort(sorted_data.begin(), sorted_data.end());
	std::pair<unsigned, unsigned> range{ 0, step_nr - 1 };	// active index-range in the beginning of HalfSampleMode
	return halfsample(sorted_data, range);
}

double mc::median(const unsigned var) const {
	std::vector<double> sorted_data;
	for (auto it : trace) sorted_data.push_back(it.at(var));
	std::sort(sorted_data.begin(), sorted_data.end());
	if (step_nr % 2 != 0) return sorted_data.at(step_nr / 2);	// odd number of datapoints
	else return (sorted_data.at((int)(step_nr / 2)-1) + sorted_data.at((int)(step_nr / 2))) / 2;	// even number of datapoints
}

void mc::reset() {
	trace.clear();
	step_nr = 0;
}
void mc::write_trace_to_file(std::ofstream &outf) const {
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
void mc::write_histogram_to_file(std::ofstream &outf, const std::pair<unsigned, unsigned> n_bins, const std::pair<unsigned, unsigned> vars) {
	std::map<std::pair<unsigned, unsigned>, unsigned> hist = histogram(n_bins, vars);
	std::pair<double, double> a{ limits[vars.first].first, limits[vars.second].first };
	std::pair<double, double> span{ spans.at(vars.first), spans.at(vars.second) };
	for (auto bin : hist) {
		std::pair<double, double> coord {
			a.first + (2 * bin.first.first - 1)*span.first / (2 * n_bins.first) ,
			a.second + (2 * bin.first.second - 1)*span.second / (2 * n_bins.second)
		};
		outf << coord.first << "\t" << coord.second << "\t" << bin.second << std::endl;
	}
}
void mc::write_autocorrelation_to_file(std::ofstream &outf, const unsigned max_lag) const {
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
