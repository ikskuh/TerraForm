#include "mainwindow.hpp"
#include "ui_mainwindow.h"
#include "featureregistry.hpp"
#include "toollistwidget.hpp"

#include "objectcontroller.hpp"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    while(ui->toolBox->count() > 0)
        ui->toolBox->removeItem(0);
    for(auto & group : featureRegistry.groups)
    {
        auto * list = new ToolListWidget;

        list->setModel(group.second.get());

        ui->toolBox->addItem(list, group.first);
    }

    this->setTerrain(std::make_unique<Terrain>(512, 512));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setTerrain(std::unique_ptr<Terrain> && terrain)
{
    this->terrain = std::move(terrain);
    this->ui->preview->setTerrain(this->terrain.get());
    this->ui->editor->setTerrain(this->terrain.get());
    this->ui->featureList->setModel(this->terrain.get());
}

void MainWindow::on_featureList_clicked(const QModelIndex &index)
{
    if(terrain and index.isValid())
    {
        ui->moveUpFeature->setEnabled(index.row() > 0);
        ui->moveDownFeature->setEnabled(index.row() < terrain->rowCount() - 1);
        ui->deleteFeature->setEnabled(true);
        this->ui->propertyGrid->setObject(terrain->feature(index));
    }
    else
    {
        this->ui->propertyGrid->setObject(nullptr);
        ui->moveUpFeature->setEnabled(false);
        ui->moveDownFeature->setEnabled(false);
        ui->deleteFeature->setEnabled(false);
    }
}

void MainWindow::on_moveDownFeature_triggered(QAction *)
{
    auto index = ui->featureList->currentIndex();
    if(not index.isValid())
        return;

    this->terrain->moveRow(
        QModelIndex(),
        index.row(),
        QModelIndex(),
        index.row() + 1
    );
}

void MainWindow::on_moveUpFeature_triggered(QAction *)
{
    auto index = ui->featureList->currentIndex();
    if(not index.isValid())
        return;

    this->terrain->moveRow(
        QModelIndex(),
        index.row(),
        QModelIndex(),
        index.row() - 1
    );
}

void MainWindow::on_deleteFeature_triggered(QAction *)
{
    auto index = ui->featureList->currentIndex();
    if(not index.isValid())
        return;
    this->terrain->removeRow(index.row());
}
