#ifndef TOOLLISTWIDGET_HPP
#define TOOLLISTWIDGET_HPP

#include <QWidget>
#include <QListView>

class ToolListWidget : public QListView
{
    Q_OBJECT
public:
    explicit ToolListWidget(QWidget *parent = nullptr);

    void startDrag(Qt::DropActions supportedActions) override;

signals:

public slots:
};

#endif // TOOLLISTWIDGET_HPP
