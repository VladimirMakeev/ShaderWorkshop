#include "effect.h"

Effect::Effect(QOpenGLShaderProgram *program, QOpenGLShader *fragmentShader,
               QOpenGLFramebufferObject *fbo, const QString &source) :
    program(program),
    fragmentShader(fragmentShader),
    framebuffer(fbo),
    inputs(4),
    fallbackSource(source),
    frame(0)
{
}

Effect::~Effect()
{
    delete program;
    delete fragmentShader;
    delete framebuffer;
}
