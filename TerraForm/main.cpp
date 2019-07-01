#include "mainwindow.hpp"
#include <QApplication>
#include <QOpenGLContext>

#include "featureregistry.hpp"
#include "features/sinewavefeature.hpp"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    {
        QSurfaceFormat format;
        format.setSamples(4);
        format.setOption(QSurfaceFormat::DebugContext, true);
        format.setOption(QSurfaceFormat::DeprecatedFunctions, false);
        format.setStereo(false);
        format.setProfile(QSurfaceFormat::CoreProfile);
        format.setVersion(4, 5);
        format.setSwapBehavior(QSurfaceFormat::SingleBuffer);
        format.setRenderableType(QSurfaceFormat::OpenGL);
        QSurfaceFormat::setDefaultFormat(format);
    }

    for(auto * it = TerrainFeatureTypeRegistrar::first; it != nullptr; it = it->next)
    {
        featureRegistry.add(it->value);
    }

    MainWindow w;
    w.show();

    return a.exec();
}
