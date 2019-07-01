#ifndef TERRAINRENDERER_HPP
#define TERRAINRENDERER_HPP

#include <QOpenGLWidget>
#include <QOpenGLFunctions_4_5_Core>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>

#include <memory>

#include "terrain.hpp"

class TerrainRenderer : public QOpenGLWidget
{
    Q_OBJECT
public:
    explicit TerrainRenderer(QWidget *parent = nullptr);

    void initializeGL() override;
    void resizeGL(int w, int h) override;

public:
    Terrain * terrain() const {
        return _terrain;
    }
    void setTerrain(Terrain * terrain);

protected:
    void updateBuffers();

private:
    void regenerateBuffers();

private:
    bool openGLready;
    Terrain * _terrain;
protected:
    QOpenGLFunctions_4_5_Core gl;
    std::unique_ptr<QOpenGLVertexArrayObject> vao;
    GLuint index_buffer, vertex_buffer;
};

#endif // TERRAINRENDERER_HPP
