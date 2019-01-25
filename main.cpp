#include "shaderworkshop.h"
#include <QApplication>
#include <QSurfaceFormat>

int main(int argc, char *argv[])
{
    // ShaderWorkshop expects OpenGL 3.3 core profile context
    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    format.setStencilBufferSize(8);
    format.setVersion(3, 3);
    format.setProfile(QSurfaceFormat::CoreProfile);

    QSurfaceFormat::setDefaultFormat(format);

    QApplication a(argc, argv);
    ShaderWorkshop w;
    w.show();

    return a.exec();
}
