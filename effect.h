#ifndef EFFECT_H
#define EFFECT_H

#include <QOpenGLShaderProgram>
#include <QOpenGLShader>
#include <QOpenGLFramebufferObject>
#include <QVector>

class Effect;

struct EffectChannelSettings
{
    EffectChannelSettings() :
        effect(Q_NULLPTR),
        filter(GL_LINEAR_MIPMAP_LINEAR),
        wrap(GL_REPEAT)
    {
    }

    /// effect used by this channel
    Effect *effect;
    /// texture filtering setting
    GLint filter;
    /// texture wrap setting
    GLint wrap;
};

class Effect
{
public:
    Effect(QOpenGLShaderProgram *program, QOpenGLShader *fragmentShader,
           QOpenGLFramebufferObject *fbo, const QString &source);

    ~Effect();

    /// shader program used by this effect
    QOpenGLShaderProgram *program;
    /// fragment shader used by this effect
    QOpenGLShader *fragmentShader;
    /// framebuffer this effect renders to
    QOpenGLFramebufferObject *framebuffer;
    /// settings for each of the input channels
    QVector<EffectChannelSettings> inputs;
    /// fragment shader source code used for fallback
    QString fallbackSource;
    /// frame counter
    int frame;
};

#endif // EFFECT_H
