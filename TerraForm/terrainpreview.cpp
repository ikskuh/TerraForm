#include "terrainpreview.hpp"

#include <QtMath>
#include <QMouseEvent>

#include <glm/glm.hpp>

static char const * const terrain_vert =
R"glsl(#version 450
    layout(location = 0) in float uVertexHeight;

    uniform mat4 uTransform;
    uniform int uTerrainSizeX;

    out vec3 world;

    void main()
    {
        int x = gl_VertexID % uTerrainSizeX;
        int y = gl_VertexID / uTerrainSizeX;

        world = vec3(
            float(x) - 0.5 * float(uTerrainSizeX),
            uVertexHeight,
            float(y) - 0.5 * float(uTerrainSizeX)
        );

        gl_Position = uTransform * vec4(world, 1.0);
    }
)glsl";

static char const * const terrain_geom =
R"glsl(#version 450
    layout(triangles) in;
    layout(triangle_strip, max_vertices = 3) out;

    in vec3[] world;

    out vec3 normal;

    void main()
    {
        vec3 dx = normalize(world[2] - world[0]);
        vec3 dy = normalize(world[1] - world[0]);

        normal = normalize(cross(dy, dx));
        for(int i = 0; i< 3; i++)
        {
            gl_Position = gl_in[i].gl_Position;
            EmitVertex();
        }
        EndPrimitive();
    }
)glsl";

static char const * const terrain_frag =
R"glsl(#version 450
    out vec4 fragment;

    in vec3 normal;

    void main()
    {
        vec3 n = normal;
        vec4 color = vec4(0.0, 0.5, 0.0, 1.0);
        if(!gl_FrontFacing) {
            n = -normal;
            color = vec4(0.5, 0.5, 0.5, 1.0);
        }

        const vec3 sun = normalize(vec3(0.3, -1.2, -0.4));

        const float ambient = 0.2;
        const float diffuse = 0.9;

        float lighting = ambient
            + diffuse * clamp(dot(n, -sun), 0.0, 1.0)
            ;

        color.rgb *= lighting;

        fragment = color;
    }
)glsl";

TerrainPreview::TerrainPreview(QWidget *parent) :
    TerrainRenderer(parent),
    pan(0.0),
    tilt(17.0),
    zoom(380)
{

}

void TerrainPreview::paintGL()
{
    gl.glClearColor(0.5f, 0.5f, 1.0f, 1.0f);
    gl.glClearDepth(1.0);
    gl.glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if(not this->terrain())
        return;

    vao->bind();
    program->bind();

    QQuaternion rot = QQuaternion::fromAxisAndAngle({0,1,0}, pan)
                    * QQuaternion::fromAxisAndAngle({1,0,0}, tilt)
                    ;

    QMatrix4x4 mat;
    mat.setToIdentity();
    mat.perspective(
        90.0,
        float(this->width()) / float(this->height()),
        0.1f,
        1000.0f
    );
    mat.lookAt(
        rot.rotatedVector(QVector3D(0, 0, -zoom)),
        QVector3D(0, 0, 0),
        rot.rotatedVector(QVector3D(0, 1, 0))
    );

    program->setUniformValue("uTransform", mat);
    program->setUniformValue("uTerrainSizeX", int(terrain()->width));

    gl.glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    gl.glDisable(GL_CULL_FACE);
    gl.glEnable(GL_DEPTH_TEST);

    gl.glDrawElements(GL_TRIANGLES, 6 * (terrain()->width - 1) * (terrain()->height - 1), GL_UNSIGNED_INT, nullptr);
}

void TerrainPreview::mousePressEvent(QMouseEvent *event)
{
    last_mousepos = event->pos();
}

void TerrainPreview::mouseMoveEvent(QMouseEvent *event)
{
    auto const dx = event->x() - last_mousepos.x();
    auto const dy = event->y() - last_mousepos.y();
    last_mousepos = event->pos();

    pan -= 1.0f * dx;
    tilt += 1.0f * dy;

    tilt = glm::clamp(tilt, -30.0f, 90.0f);

    this->update();
}

void TerrainPreview::wheelEvent(QWheelEvent *event)
{
    zoom -= 0.5f * event->delta();
    zoom = glm::clamp(zoom, 10.0f, 500.0f);
    this->update();
}

void TerrainPreview::initializeGL()
{
    TerrainRenderer::initializeGL();

    program = std::make_unique<QOpenGLShaderProgram>();
    program->addShaderFromSourceCode(QOpenGLShader::Vertex, terrain_vert);
    program->addShaderFromSourceCode(QOpenGLShader::Geometry, terrain_geom);
    program->addShaderFromSourceCode(QOpenGLShader::Fragment, terrain_frag);
    program->link();
}

