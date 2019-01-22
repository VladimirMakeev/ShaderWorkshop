#ifndef SHADERWORKSHOP_H
#define SHADERWORKSHOP_H

#include <QWidget>

namespace Ui {
class ShaderWorkshop;
}

class ShaderWorkshop : public QWidget
{
    Q_OBJECT

public:
    explicit ShaderWorkshop(QWidget *parent = 0);
    ~ShaderWorkshop();

private:
    Ui::ShaderWorkshop *ui;
};

#endif // SHADERWORKSHOP_H
