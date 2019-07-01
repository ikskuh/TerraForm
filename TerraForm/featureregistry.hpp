#ifndef FEATUREREGISTRY_HPP
#define FEATUREREGISTRY_HPP

#include <map>
#include <memory>

#include "terrainfeaturegroup.hpp"

struct FeatureRegistry
{
    std::map<QString, std::unique_ptr<TerrainFeatureGroup>> groups;

    void add(TerrainFeatureType const & type);
};

extern FeatureRegistry featureRegistry;

#endif // FEATUREREGISTRY_HPP
