#ifndef TERRAINDESIGNER_HPP
#define TERRAINDESIGNER_HPP

#include "terrainrenderer.hpp"

#include <glm/glm.hpp>

class TerrainDesigner : public TerrainRenderer
{
    Q_OBJECT
public:
    explicit TerrainDesigner(QWidget *parent = nullptr);

    void initializeGL() override;
    void paintGL() override;

    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;

private:
    std::unique_ptr<QOpenGLShaderProgram> surface_program, vertex_program;
    QPoint last_mousepos;

    glm::vec2 terrain_pan;
    float terrain_zoom;

    QMatrix4x4 get_transform() const;

    QVector3D screen_to_world(int x, int y) const;
};

#endif // TERRAINDESIGNER_HPP
