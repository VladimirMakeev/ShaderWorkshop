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
