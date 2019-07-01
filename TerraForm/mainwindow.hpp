#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <memory>

#include "terrain.hpp"

namespace Ui {
class MainWindow;
}

class ObjectController;

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    Ui::MainWindow *ui;
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_featureList_clicked(const QModelIndex &index);

    void on_moveDownFeature_triggered(QAction *arg1);

    void on_moveUpFeature_triggered(QAction *arg1);

    void on_deleteFeature_triggered(QAction *arg1);

private:
    void setTerrain(std::unique_ptr<Terrain> && terrain);

private:
    std::unique_ptr<Terrain> terrain;
    ObjectController * controller;
};

#endif // MAINWINDOW_HPP
