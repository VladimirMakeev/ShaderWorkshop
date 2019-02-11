/*
 * This file is part of ShaderWorkshop (https://github.com/VladimirMakeev/ShaderWorkshop).
 * Copyright (C) 2019 Vladimir Makeev.
 *
 * ShaderWorkshop is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * ShaderWorkshop is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General Public License
 * along with ShaderWorkshop.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef RENDERER_H
#define RENDERER_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLShader>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QHash>
#include <QElapsedTimer>
#include <QTimer>
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

public slots:
    void effectInputChanged(int index, int channel, int effectIndex);
    void effectFilteringChanged(int index, int channel, GLint value);
    void effectWrapChanged(int index, int channel, GLint value);

protected:
    void initializeGL() Q_DECL_OVERRIDE;
    void resizeGL(int w, int h) Q_DECL_OVERRIDE;
    void paintGL() Q_DECL_OVERRIDE;

    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

private:
    void setupVertexShader();
    void setupBuffers();

    Effect* createEffect();
    void renderEffects();
    void renderMainImage();
    void renderEffect(Effect &effect, QSize textureSize);
    void removeEffectFromInputs(const Effect *effect);
    /// bind textures according to this effect input channels settings,
    /// set sampler settings
    void bindEffectTextures(const Effect &effect);
    void setSamplingParameters(const EffectChannelSettings &settings);
    void setUniforms(const Effect &effect, QSize textureSize);
    void convertPointToOpenGl(QPoint &point) const;

    QHash<int, Effect*> effects;
    Effect *mainImage;
    /// vertex shader used for all effects
    QOpenGLShader *vertexShader;
    QTimer *updateTimer;

    QOpenGLVertexArrayObject vao;
    QOpenGLBuffer vbo;
    QElapsedTimer timer;
    /// mouse pixel coordinates, xy: current if left button down, zw: click
    QVector4D mouse;
    QSize fboTextureSize;
    QSize viewSize;
    int fps;
};

#endif // RENDERER_H
