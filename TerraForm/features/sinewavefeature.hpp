#ifndef SINEWAVEFEATURE_HPP
#define SINEWAVEFEATURE_HPP

#include "../terrainfeature.hpp"


struct SineWaveFeature : TerrainFeature
{
    Q_OBJECT
public:
    enum SineWaveMode {
        Linear,
        Centroid,
    };
    Q_ENUM(SineWaveMode)
public:
    SineWaveFeature();

    float x, y;

    SineWaveMode mode = Linear;
    qreal direction = 0.0;
    qreal amplitude = 60.0;
    qreal frequency = 1.00;
    qreal phase_shift = 0.0;

    void apply(Terrain & terrain) const override;


    Q_PROPERTY(SineWaveMode Mode       MEMBER mode       NOTIFY propertyChanged)
    Q_PROPERTY(qreal        Direction  MEMBER direction   NOTIFY propertyChanged)
    Q_PROPERTY(qreal        Amplitude  MEMBER amplitude   NOTIFY propertyChanged)
    Q_PROPERTY(qreal        Frequency  MEMBER frequency   NOTIFY propertyChanged)
    Q_PROPERTY(qreal        PhaseShift MEMBER phase_shift NOTIFY propertyChanged)
};

#endif // SINEWAVEFEATURE_HPP
