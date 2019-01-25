#include "renderer.h"

Renderer::Renderer(QWidget *parent) : QOpenGLWidget(parent)
{

}

Renderer::~Renderer()
{

}

void Renderer::initializeGL()
{
    initializeOpenGLFunctions();

    glClearColor(1.0f, 0.0f, 0.4f, 1.0f);
}

void Renderer::resizeGL(int w, int h)
{
    QOpenGLWidget::resizeGL(w, h);
}

void Renderer::paintGL()
{
    QOpenGLWidget::paintGL();
}
