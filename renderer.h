#ifndef RENDERER_H
#define RENDERER_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLShader>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include "effect.h"

class Renderer : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit Renderer(QWidget *parent = Q_NULLPTR);
    virtual ~Renderer();

protected:
    void initializeGL() Q_DECL_OVERRIDE;
    void resizeGL(int w, int h) Q_DECL_OVERRIDE;
    void paintGL() Q_DECL_OVERRIDE;

private:
    QString defaultFragmentShader() const;
    void setupVertexShader();
    void setupBuffers();

    Effect* createEffect();
    void renderEffect(Effect &effect, QSize textureSize);
    /// bind textures according to this effect input channels settings,
    /// set sampler settings
    void bindEffectTextures(const Effect &effect);

    Effect *mainImage;
    /// vertex shader used for all effects
    QOpenGLShader *vertexShader;

    QOpenGLVertexArrayObject vao;
    QOpenGLBuffer vbo;

    QSize fboTextureSize;
    QSize viewSize;
};

#endif // RENDERER_H
