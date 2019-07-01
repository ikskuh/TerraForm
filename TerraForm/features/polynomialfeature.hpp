#ifndef POLYNOMIALFEATURE_HPP
#define POLYNOMIALFEATURE_HPP

#include "../terrainfeature.hpp"

struct PolynomialFeature : TerrainFeature
{
    Q_OBJECT
public:
    enum PolynomialMode {
        Linear,
        Centroid,
    };
    Q_ENUM(PolynomialMode)
public:
    PolynomialFeature();

    float x, y;

    PolynomialMode mode = Linear;
    qreal direction = 0.0;
    qreal constant = 0.0;
    qreal linear = 0.0;
    qreal quadratic = 0.0;
    qreal cubic = 0.00;

    void apply(Terrain & terrain) const override;

    Q_PROPERTY(PolynomialMode Mode      MEMBER mode      NOTIFY propertyChanged)
    Q_PROPERTY(qreal          Direction MEMBER direction NOTIFY propertyChanged)
    Q_PROPERTY(qreal          Constant  MEMBER constant  NOTIFY propertyChanged)
    Q_PROPERTY(qreal          Linear    MEMBER linear    NOTIFY propertyChanged)
    Q_PROPERTY(qreal          Quadratic MEMBER quadratic NOTIFY propertyChanged)
    Q_PROPERTY(qreal          Cubic     MEMBER cubic     NOTIFY propertyChanged)
};

#endif // POLYNOMIALFEATURE_HPP
