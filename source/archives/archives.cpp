#include "archives\archives.hpp"

// mc

template<class Archive>
void mc_archive::serialize(Archive & ar) {
	ar(gen_status, x, step_nr, limits, trace); // serialize things by passing them to the archive
}
template void mc_archive::serialize<cereal::BinaryInputArchive>(cereal::BinaryInputArchive & archive);
template void mc_archive::serialize<cereal::BinaryOutputArchive>(cereal::BinaryOutputArchive & archive);

// inference

template<class Archive>
void inference_archive::serialize(Archive & ar) {
	ar(mcdata, prior_distributions, observations, param_attributes, proposal_width); // serialize things by passing them to the archive
}
template void inference_archive::serialize<cereal::BinaryInputArchive>(cereal::BinaryInputArchive & archive);
template void inference_archive::serialize<cereal::BinaryOutputArchive>(cereal::BinaryOutputArchive & archive);

// metropolis

template<class Archive>
void metropolis_archive::serialize(Archive & ar) {
	ar(mcdata, proposal_width); // serialize things by passing them to the archive
}
template void metropolis_archive::serialize<cereal::BinaryInputArchive>(cereal::BinaryInputArchive & archive);
template void metropolis_archive::serialize<cereal::BinaryOutputArchive>(cereal::BinaryOutputArchive & archive);
