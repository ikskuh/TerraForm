#include "toollistwidget.hpp"
#include "dragdrop.hpp"
#include "terrainfeaturegroup.hpp"

#include <QMimeData>
#include <QDrag>

ToolListWidget::ToolListWidget(QWidget *parent) :
    QListView(parent)
{
    this->setSelectionMode(QAbstractItemView::SingleSelection);
    this->setDragEnabled(true);
    this->setDefaultDropAction(Qt::CopyAction);
    this->setAcceptDrops(false);
    this->setDropIndicatorShown(true);
}

void ToolListWidget::startDrag(Qt::DropActions supportedActions)
{
    auto group = static_cast<TerrainFeatureGroup*>(model());
    if(not group)
        return;

    TerrainFeatureType const * item = qvariant_cast<TerrainFeatureType const *>(group->data(currentIndex(), Qt::UserRole));

    QMimeData* mimeData = new QMimeData;

    QByteArray payload;
    payload.resize(sizeof(item));
    reinterpret_cast<TerrainFeatureType const * &>(*payload.data()) = item;

    mimeData->setData(DragDrop::FeatureFormat, payload);

    QDrag* drag = new QDrag(this);
    drag->setMimeData(mimeData);
    if (drag->exec(Qt::CopyAction) == Qt::CopyAction) {
        // delete takeItem(row(item));
        // emit itemDroped();
    }
}
