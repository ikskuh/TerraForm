#ifndef TERRAIN_HPP
#define TERRAIN_HPP

#include <vector>
#include <memory>
#include <QObject>
#include <QAbstractListModel>

#include "terrainfeature.hpp"

struct Terrain : public QAbstractListModel
{
    Q_OBJECT
private:
    std::vector<std::unique_ptr<TerrainFeature>> features;
public:

    explicit Terrain() = delete;
    explicit Terrain(size_t w, size_t h);
    Terrain(Terrain const &) = delete;
    Terrain(Terrain &&) = delete;

    size_t width, height;
    std::vector<float> heightmap;


    void recalculate();

    float & operator() (size_t x, size_t y) {
        return heightmap.at(width * y + x);
    }

    float operator() (size_t x, size_t y) const {
        return heightmap.at(width * y + x);
    }

    void add(std::unique_ptr<TerrainFeature> && feature);

signals:
    void dataChanged();

public:

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;

    Qt::DropActions supportedDragActions() const override;
    Qt::DropActions supportedDropActions() const override;

    TerrainFeature * feature(QModelIndex const & index);
};

#endif // TERRAIN_HPP
