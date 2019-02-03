#ifndef RENDERER_H
#define RENDERER_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLShader>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QHash>
#include <QElapsedTimer>
#include "effect.h"

class Renderer : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit Renderer(QWidget *parent = Q_NULLPTR);
    virtual ~Renderer();

    QString defaultFragmentShader() const;

    /// create new effect with specified index
    void createEffect(int index);
    /// delete existing effect with specified index
    void deleteEffect(int index);
    /// recompile fragment shader for effect
    QString recompileEffectShader(int index, const QString &source);

protected:
    void initializeGL() Q_DECL_OVERRIDE;
    void resizeGL(int w, int h) Q_DECL_OVERRIDE;
    void paintGL() Q_DECL_OVERRIDE;

private:
    void setupVertexShader();
    void setupBuffers();

    Effect* createEffect();
    void renderEffects();
    void renderMainImage();
    void renderEffect(Effect &effect, QSize textureSize);
    /// bind textures according to this effect input channels settings,
    /// set sampler settings
    void bindEffectTextures(const Effect &effect);
    void setUniforms(const Effect &effect, QSize textureSize);

    QHash<int, Effect*> effects;
    Effect *mainImage;
    /// vertex shader used for all effects
    QOpenGLShader *vertexShader;

    QOpenGLVertexArrayObject vao;
    QOpenGLBuffer vbo;
    QElapsedTimer timer;

    QSize fboTextureSize;
    QSize viewSize;
};

#endif // RENDERER_H
