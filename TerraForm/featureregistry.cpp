#include "featureregistry.hpp"

FeatureRegistry featureRegistry;

void FeatureRegistry::add(const TerrainFeatureType &type)
{
    auto & group = groups[type.category];

    if(not group) group = std::make_unique<TerrainFeatureGroup>();

    group->add(type);
}
