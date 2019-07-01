#ifndef TERRAINPREVIEW_HPP
#define TERRAINPREVIEW_HPP

#include "terrainrenderer.hpp"

class TerrainPreview : public TerrainRenderer
{
    Q_OBJECT
public:
    explicit TerrainPreview(QWidget *parent = nullptr);

    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

    void initializeGL() override;
    void paintGL() override;

private:
    std::unique_ptr<QOpenGLShaderProgram> program;
    float pan, tilt, zoom;
    QPoint last_mousepos;
};

#endif // TERRAINPREVIEW_HPP
