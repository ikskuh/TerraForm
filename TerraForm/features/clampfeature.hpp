#ifndef CLAMPFEATURE_HPP
#define CLAMPFEATURE_HPP

#include "../terrainfeature.hpp"


struct ClampFeature : TerrainFeature
{
    Q_OBJECT
public:
    float x, y;

    qreal lower_bound = 0.0;
    qreal upper_bound = 0.0;

    bool clamp_upper = false;
    bool clamp_lower = false;

    qreal smooth_radius = 10.0;
    bool use_smooth = false;

    void apply(Terrain & terrain) const override;

    Q_PROPERTY(bool  ClampBottom MEMBER clamp_lower   NOTIFY propertyChanged)
    Q_PROPERTY(qreal LowerBound MEMBER lower_bound   NOTIFY propertyChanged)

    Q_PROPERTY(bool  ClampTop MEMBER clamp_upper   NOTIFY propertyChanged)
    Q_PROPERTY(qreal UpperBound MEMBER upper_bound NOTIFY propertyChanged)

    Q_PROPERTY(bool  Smooth   MEMBER use_smooth   NOTIFY propertyChanged)
    Q_PROPERTY(qreal Radius   MEMBER smooth_radius   NOTIFY propertyChanged)
};

#endif // CLAMPFEATURE_HPP
