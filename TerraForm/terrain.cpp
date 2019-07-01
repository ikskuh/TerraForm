#include "terrain.hpp"
#include <cmath>

Terrain::Terrain(size_t w, size_t h)
    : width(w), height(h), heightmap(w * h)
{

}

void Terrain::recalculate()
{
    for(auto & vertex : heightmap)
        vertex = 0.0f;

    for(auto const & feature : features)
        feature->apply(*this);

    emit dataChanged();
}

void Terrain::add(std::unique_ptr<TerrainFeature> &&feature)
{
    connect(feature.get(), &TerrainFeature::propertyChanged, this, &Terrain::recalculate);

    this->beginInsertRows(QModelIndex{}, rowCount(), rowCount());
    this->features.emplace_back(std::move(feature));
    this->endInsertRows();
}


int Terrain::rowCount(const QModelIndex &parent) const
{
    return features.size();
}

QVariant Terrain::data(const QModelIndex &index, int role) const
{
    auto const & val = features.at(index.row());
    switch(role) {
    case Qt::DisplayRole: return val->objectName();
    // case Qt::DecorationRole: return val.icon;
    // case Qt::UserRole: return QVariant::fromValue(&val);
    }
    return QVariant { };
}

Qt::ItemFlags Terrain::flags(const QModelIndex &index) const
{
    Qt::ItemFlags defaultFlags = QAbstractListModel::flags(index);
    if (index.isValid())
        return Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | defaultFlags;
    else
        return Qt::ItemIsDropEnabled | defaultFlags;
}

Qt::DropActions Terrain::supportedDragActions() const
{
    return Qt::MoveAction;
}

Qt::DropActions Terrain::supportedDropActions() const
{
    return Qt::MoveAction;
}

TerrainFeature *Terrain::feature(const QModelIndex &index)
{
    return features.at(index.row()).get();
}
