#include "terrainrenderer.hpp"

#include <iostream>


static void DebugMsg(GLenum source,GLenum type,GLuint id,GLenum severity,GLsizei length,const GLchar *message,const void *userParam)
{
    std::cerr << "OpenGL Error: " << std::string(message, length) << std::endl;
}

TerrainRenderer::TerrainRenderer(QWidget *parent) :
    QOpenGLWidget(parent),
    openGLready(false),
    _terrain(nullptr),
    index_buffer(0),
    vertex_buffer(0)
{

}



void TerrainRenderer::initializeGL()
{
    if(gl.initializeOpenGLFunctions() == false)
        qDebug() << "failed to initialize OpenGL functions!";

    gl.glDebugMessageCallback(DebugMsg, nullptr);

    vao = std::make_unique<QOpenGLVertexArrayObject>();
    vao->create();
    vao->bind();

    gl.glEnableVertexArrayAttrib(vao->objectId(), 0);
    gl.glVertexArrayAttribFormat(
        vao->objectId(),
        0,
        1, GL_FLOAT,
        GL_FALSE,
        0
    );

    gl.glVertexArrayAttribBinding(
        vao->objectId(),
        0,
        0
    );

    openGLready = true;

    this->regenerateBuffers();
}

void TerrainRenderer::resizeGL(int w, int h)
{
    gl.glViewport(0, 0, w, h);
}


void TerrainRenderer::setTerrain(Terrain * terrain)
{
    if(this->_terrain) {
        disconnect(this->_terrain, &Terrain::dataChanged, this, &TerrainRenderer::updateBuffers);
    }

    this->_terrain = terrain;
    this->regenerateBuffers();

    connect(this->_terrain, &Terrain::dataChanged, this, &TerrainRenderer::updateBuffers);

}

void TerrainRenderer::updateBuffers()
{
    if(not this->_terrain)
        return;
    if(not openGLready)
        return;
    gl.glNamedBufferSubData(
        vertex_buffer,
        0,
        sizeof(terrain()->heightmap[0]) * terrain()->heightmap.size(),
        terrain()->heightmap.data()
    );
    update();
}

void TerrainRenderer::regenerateBuffers()
{
    if(not this->_terrain)
        return;
    if(not openGLready)
        return;

    size_t const w = this->_terrain->width;
    size_t const h = this->_terrain->height;

    std::vector<uint32_t> indices(6 * (w-1) * (h-1));

    for(size_t y = 0; y < (h - 1); y++)
    {
        for(size_t x = 0; x < (w - 1); x++)
        {
            uint32_t * base = indices.data() + 6 * ((w-1) * y + x);

            uint32_t const i00 = (y+0) * w + (x+0);
            uint32_t const i01 = (y+0) * w + (x+1);
            uint32_t const i10 = (y+1) * w + (x+0);
            uint32_t const i11 = (y+1) * w + (x+1);

            base[0] = i00;
            base[1] = i10;
            base[2] = i11;
            base[3] = i01;
            base[4] = i00;
            base[5] = i11;
        }
    }

    makeCurrent();

    if(index_buffer != 0)
        gl.glDeleteBuffers(1, &index_buffer);
    if(vertex_buffer != 0)
        gl.glDeleteBuffers(1, &vertex_buffer);

    gl.glCreateBuffers(1, &index_buffer);
    gl.glCreateBuffers(1, &vertex_buffer);

    gl.glNamedBufferStorage(
        index_buffer,
        sizeof(uint32_t) * indices.size(),
        indices.data(),
        0
    );

    gl.glNamedBufferStorage(
        vertex_buffer,
        sizeof(_terrain->heightmap[0]) * _terrain->heightmap.size(),
        _terrain->heightmap.data(),
        GL_DYNAMIC_STORAGE_BIT
    );

    gl.glVertexArrayVertexBuffer(vao->objectId(), 0, vertex_buffer, 0, sizeof(_terrain->heightmap[0]));
    gl.glVertexArrayElementBuffer(vao->objectId(), index_buffer);

    this->index_buffer = std::move(index_buffer);

    update();
}
