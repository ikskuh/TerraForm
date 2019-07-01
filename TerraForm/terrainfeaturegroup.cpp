#include "terrainfeaturegroup.hpp"

TerrainFeatureGroup::TerrainFeatureGroup(QObject *parent) :
    QAbstractListModel(parent)
{

}

int TerrainFeatureGroup::rowCount(const QModelIndex &parent) const
{
    return types.size();
}

QVariant TerrainFeatureGroup::data(const QModelIndex &index, int role) const
{
    auto const & val = types.at(index.row());
    switch(role) {
    case Qt::DisplayRole: return val.displayName;
    case Qt::DecorationRole: return QIcon(val.icon);
    case Qt::UserRole: return QVariant::fromValue(&val);
    }
    return QVariant { };
}

Qt::ItemFlags TerrainFeatureGroup::flags(const QModelIndex &index) const
{
    Qt::ItemFlags defaultFlags = QAbstractListModel::flags(index);
    if (index.isValid())
        return Qt::ItemIsDragEnabled | defaultFlags;
    else
        return defaultFlags;
}

Qt::DropActions TerrainFeatureGroup::supportedDragActions() const
{
    return Qt::CopyAction;
}

void TerrainFeatureGroup::add(TerrainFeatureType const & type)
{
    this->beginInsertRows(QModelIndex{}, rowCount(), rowCount());
    this->types.emplace_back(type);
    this->endInsertRows();
}
