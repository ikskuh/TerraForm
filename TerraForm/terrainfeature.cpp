#include "terrainfeature.hpp"

TerrainFeature::TerrainFeature()
{
    this->setObjectName(this->metaObject()->className());
}

TerrainFeature::~TerrainFeature()
{

}

TerrainFeatureTypeRegistrar * TerrainFeatureTypeRegistrar::first = nullptr;


TerrainFeatureTypeRegistrar::TerrainFeatureTypeRegistrar(const TerrainFeatureType &type) :
    next(first),
    value(type)
{
    first = this;
}
