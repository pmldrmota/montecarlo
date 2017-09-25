#include "archives.hpp"

// mc

template<class Archive>
void mc_archive::serialize(Archive & ar) {
	ar(gen_status, x, limits, trace); // serialize things by passing them to the archive
}
template void mc_archive::serialize<cereal::BinaryInputArchive>(cereal::BinaryInputArchive & archive);
template void mc_archive::serialize<cereal::BinaryOutputArchive>(cereal::BinaryOutputArchive & archive);

// inference

template<class Archive>
void inference_archive::serialize(Archive & ar) {
	ar(mcdata, prior_distributions, observations, proposal_width); // serialize things by passing them to the archive
}
template void inference_archive::serialize<cereal::BinaryInputArchive>(cereal::BinaryInputArchive & archive);
template void inference_archive::serialize<cereal::BinaryOutputArchive>(cereal::BinaryOutputArchive & archive);

// bimodal

template<class Archive>
void bimodal_archive::serialize(Archive & ar) {
	ar(mcdata, proposal_width); // serialize things by passing them to the archive
}
template void bimodal_archive::serialize<cereal::BinaryInputArchive>(cereal::BinaryInputArchive & archive);
template void bimodal_archive::serialize<cereal::BinaryOutputArchive>(cereal::BinaryOutputArchive & archive);