#include "terraindesigner.hpp"
#include "dragdrop.hpp"
#include "terrainfeaturegroup.hpp"

#include <QDebug>

#include <QtMath>
#include <QMouseEvent>
#include <QMimeData>

static char const * const terrain_vert =
R"glsl(#version 450
    layout(location = 0) in float uVertexHeight;

    uniform mat4 uTransform;
    uniform int uTerrainSizeX;

    out float height;

    void main()
    {
        int x = gl_VertexID % uTerrainSizeX;
        int y = gl_VertexID / uTerrainSizeX;

        vec3 world = vec3(
            float(x),
            float(y),
            0.0
        );

        height = uVertexHeight;

        gl_Position = uTransform * vec4(world, 1.0);
        gl_PointSize = 1.0;
    }
)glsl";

static char const * const terrain_frag =
R"glsl(#version 450
    out vec4 fragment;

    // in vec3 normal;
    in float height;

    void main()
    {
        fragment = vec4(vec3(0.5 + 0.01 * height), 1.0);
    }
)glsl";

static char const * const terrain_vertex_frag =
R"glsl(#version 450
    out vec4 fragment;

    uniform float uAlpha;

    void main()
    {
        fragment = vec4(1, 0, 0, uAlpha);
    }
)glsl";

TerrainDesigner::TerrainDesigner(QWidget *parent) :
    TerrainRenderer(parent),
    terrain_pan(0, 0),
    terrain_zoom(1.1)
{

}

void TerrainDesigner::initializeGL()
{
    TerrainRenderer::initializeGL();

    surface_program = std::make_unique<QOpenGLShaderProgram>();
    surface_program->addShaderFromSourceCode(QOpenGLShader::Vertex, terrain_vert);
    surface_program->addShaderFromSourceCode(QOpenGLShader::Fragment, terrain_frag);
    surface_program->link();

    vertex_program = std::make_unique<QOpenGLShaderProgram>();
    vertex_program->addShaderFromSourceCode(QOpenGLShader::Vertex, terrain_vert);
    vertex_program->addShaderFromSourceCode(QOpenGLShader::Fragment, terrain_vertex_frag);
    vertex_program->link();
}

void TerrainDesigner::paintGL()
{
    gl.glClearColor(0.5f, 0.5f, 1.0f, 1.0f);
    gl.glClearDepth(1.0);
    gl.glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if(not this->terrain())
        return;

    vao->bind();
    auto const mat = get_transform();

    gl.glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    gl.glDisable(GL_CULL_FACE);
    gl.glDisable(GL_DEPTH_TEST);
    gl.glEnable(GL_PROGRAM_POINT_SIZE);

    surface_program->bind();
    surface_program->setUniformValue("uTransform", mat);
    surface_program->setUniformValue("uTerrainSizeX", int(terrain()->width));

    gl.glDrawElements(GL_TRIANGLES, 6 * (terrain()->width - 1) * (terrain()->height - 1), GL_UNSIGNED_INT, nullptr);

    // render the terrain vertices
    if(terrain_zoom >= 3.5f)
    {
        gl.glEnable(GL_BLEND);
        gl.glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        gl.glBlendEquation(GL_ADD);
        vertex_program->bind();
        vertex_program->setUniformValue("uTransform", mat);
        vertex_program->setUniformValue("uTerrainSizeX", int(terrain()->width));
        vertex_program->setUniformValue("uAlpha", glm::clamp(2.0f * (terrain_zoom - 3.5f), 0.0f, 1.0f));

        gl.glDrawArrays(GL_POINTS, 0, terrain()->width * terrain()->height);
        gl.glDisable(GL_BLEND);
    }
}

void TerrainDesigner::mousePressEvent(QMouseEvent *event)
{
    last_mousepos = event->pos();
}

QVector3D TerrainDesigner::screen_to_world(int x, int y) const
{
    auto const mat = get_transform().inverted();

    float const fx = 2.0f * float(x) / float(this->width() - 1) - 1.0f;
    float const fy = 1.0f - 2.0f * float(y) / float(this->height() - 1);

    QVector4D const screen_space { fx, fy, 0.0, 1.0 };

    auto const world_space = mat * screen_space;

    return QVector3D {
        world_space.x() / world_space.w(),
        world_space.y() / world_space.w(),
        world_space.z() / world_space.w(),
    };
}

void TerrainDesigner::mouseMoveEvent(QMouseEvent *event)
{
    auto const _new  = screen_to_world(event->x(), event->y());
    auto const _last = screen_to_world( last_mousepos.x(), last_mousepos.y());

    auto const delta = _new - _last;

    last_mousepos = event->pos();

    terrain_pan.x += delta.x();
    terrain_pan.y += delta.y();

    update();
}

void TerrainDesigner::wheelEvent(QWheelEvent *event)
{
    terrain_zoom += event->delta() / 1200.0f;

    terrain_zoom = glm::clamp(terrain_zoom, 0.5f, 5.0f);

    update();
}

void TerrainDesigner::dragEnterEvent(QDragEnterEvent *event)
{
    if (terrain() and event->mimeData()->hasFormat(DragDrop::FeatureFormat))
    {
        event->setDropAction(Qt::CopyAction);
        event->accept();
    } else {
        event->ignore();
    }
}

void TerrainDesigner::dropEvent(QDropEvent *event)
{
    if (terrain() and event->mimeData()->hasFormat(DragDrop::FeatureFormat)) {

        auto const payload = event->mimeData()->data(DragDrop::FeatureFormat);

        auto const feature = reinterpret_cast<TerrainFeatureType const * const &>(*payload.data());

        auto const pos = screen_to_world(event->pos().x(), event->pos().y());

        terrain()->add(std::unique_ptr<TerrainFeature>(feature->factory(
            *terrain(),
            pos.x(),
            pos.y())
        ));

        terrain()->recalculate();

        event->setDropAction(Qt::CopyAction);
        event->accept();
    } else {
        event->ignore();
    }
}

QMatrix4x4 TerrainDesigner::get_transform() const
{
    if(terrain() == nullptr)
        return QMatrix4x4();

    float w = terrain()->width;
    float h = terrain()->height;

    float a1 = w / h;
    float a2 = float(this->width()) / float(this->height());


    if(a1 > a2) {
        h /= a2;
    } else {
        w *= a2;
    }

    w /= 2.0f;
    h /= 2.0f;


    QMatrix4x4 mat;
    mat.setToIdentity();
    mat.ortho(-w, w, -h, h, 0.0f, 1.0f);

    mat.scale(terrain_zoom, terrain_zoom, terrain_zoom);

    mat.translate(terrain_pan.x, terrain_pan.y, 0.0);

    mat.translate(
        -float(terrain()->width) / 2.0f,
        -float(terrain()->height) / 2.0f,
        0.0f
    );

    return mat;
}

