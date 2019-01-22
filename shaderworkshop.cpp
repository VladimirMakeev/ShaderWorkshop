#include "shaderworkshop.h"
#include "ui_shaderworkshop.h"

ShaderWorkshop::ShaderWorkshop(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ShaderWorkshop)
{
    ui->setupUi(this);
}

ShaderWorkshop::~ShaderWorkshop()
{
    delete ui;
}
