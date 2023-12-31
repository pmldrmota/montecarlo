#ifndef archives_hpp_
#define archives_hpp_
#ifndef __USE_MINGW_ANSI_STDIO
#define __USE_MINGW_ANSI_STDIO 0
#endif // !__USE_MINGW_ANSI_STDIO

#include "cereal\archives\binary.hpp"
#include "cereal\types\utility.hpp"
#include "cereal\types\tuple.hpp"
#include "cereal\types\vector.hpp"
#include "cereal\types\string.hpp"
#include "common\dist_type.h"

#include <string>
#include <vector>
#include <tuple>

struct mc_archive {
	/*
	// for re-construction of mc instance from cereal binary file: use constructor with this 'archive' structure as its argument
	*/
	std::string gen_status;
	std::vector<double> x;
	unsigned step_nr;
	std::vector< std::pair<double, double> > limits;
	std::vector<std::vector<double>> trace;
	// CEREAL
	template<class Archive>
	void serialize(Archive & ar); // serialize things by passing them to the archive
};
struct inference_archive {
	/*
	// for re-construction of mc instance from cereal binary file: use constructor with this 'archive' structure as its argument
	*/
	mc_archive mcdata;

	std::vector<std::tuple<dist_type, double, double>> prior_distributions;
	std::vector<std::vector<double>> observations;
	std::vector<param_type> param_attributes;
	double proposal_width;

	// CEREAL
	template<class Archive>
	void serialize(Archive & ar); // serialize things by passing them to the archive
};
struct metropolis_archive {
	/*
	// for re-construction of mc instance from cereal binary file: use constructor with this 'archive' structure as its argument
	*/
	mc_archive mcdata;

	double proposal_width;

	// CEREAL
	template<class Archive>
	void serialize(Archive & ar); // serialize things by passing them to the archive
};

#endif // !archives_hpp_
