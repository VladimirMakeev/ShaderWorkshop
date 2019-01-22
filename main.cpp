#include "shaderworkshop.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ShaderWorkshop w;
    w.show();

    return a.exec();
}
