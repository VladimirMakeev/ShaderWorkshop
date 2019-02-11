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
