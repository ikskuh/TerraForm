#ifndef TERRAINFEATUREGROUP_HPP
#define TERRAINFEATUREGROUP_HPP

#include "terrainfeature.hpp"

#include <functional>
#include <QAbstractListModel>

class TerrainFeatureGroup : public QAbstractListModel
{
    Q_OBJECT
    std::vector<TerrainFeatureType> types;
public:
    explicit TerrainFeatureGroup(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;

    Qt::DropActions supportedDragActions() const override;



    QString name;

    void add(TerrainFeatureType const & type);
};

#endif // TERRAINFEATUREGROUP_HPP
