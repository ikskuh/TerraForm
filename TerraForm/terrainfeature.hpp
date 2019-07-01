#ifndef TERRAINFEATURE_HPP
#define TERRAINFEATURE_HPP

#include <QString>
#include <QObject>
#include <memory>

struct Terrain;

struct TerrainFeature : QObject
{
    Q_OBJECT
public:
    TerrainFeature();
    virtual ~TerrainFeature();

    virtual void apply(Terrain & terrain) const = 0;

signals:
    void propertyChanged();
};

#include <functional>

#include <QObject>
#include <QAbstractListModel>
#include <QIcon>

struct TerrainFeatureType
{
    QString category;
    QString displayName;
    QString icon;
    std::function<TerrainFeature*(Terrain const & terrain, float x, float y)> factory;
};

Q_DECLARE_METATYPE(TerrainFeatureType const *)

struct TerrainFeatureTypeRegistrar
{
    friend int main(int argc, char ** argv);
private:
    static TerrainFeatureTypeRegistrar * first;
    TerrainFeatureTypeRegistrar * next;
    TerrainFeatureType value;
public:
    explicit TerrainFeatureTypeRegistrar(TerrainFeatureType const & type);
};

#define REGISTER_TERRAIN_FEATURE(_Category, _Name, _Icon, _FunctionBody) \
    static TerrainFeatureTypeRegistrar _TerrainType_Registrar##__COUNTER__ { \
        TerrainFeatureType { \
            _Category, \
            _Name, \
            _Icon, \
            [](Terrain const & terrain, float x, float y) -> TerrainFeature* \
                _FunctionBody \
    } \
}

#endif // TERRAINFEATURE_HPP
