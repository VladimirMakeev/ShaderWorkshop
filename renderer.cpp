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

#include "renderer.h"
#include <QMouseEvent>

Renderer::Renderer(QWidget *parent) :
    QOpenGLWidget(parent),
    mainImage(Q_NULLPTR),
    updateTimer(new QTimer(this)),
    fboTextureSize(1024, 768),
    fps(60)
{
    timer.start();
}

Renderer::~Renderer()
{
    makeCurrent();

    vbo.release();
    vao.release();

    delete vertexShader;
    qDeleteAll(effects);

    doneCurrent();
}

void Renderer::initializeGL()
{
    initializeOpenGLFunctions();

    glClearColor(1.0f, 0.0f, 0.4f, 1.0f);

    setupVertexShader();

    setupBuffers();

    connect(updateTimer, SIGNAL(timeout()), this, SLOT(update()));
    updateTimer->start(1000.0 / fps);
}

void Renderer::resizeGL(int w, int h)
{
    viewSize = QSize(w, h);
    QOpenGLWidget::resizeGL(w, h);
}

void Renderer::paintGL()
{
    // there is no reason to render at all if we don't have main image
    if (!mainImage) {
        return;
    }

    renderEffects();

    renderMainImage();
}

void Renderer::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        QPoint pos = event->pos();

        convertPointToOpenGl(pos);

        mouse = QVector4D(pos.x(), pos.y(), pos.x(), pos.y());
    }
}

void Renderer::mouseMoveEvent(QMouseEvent *event)
{
    QPoint pos = event->pos();

    convertPointToOpenGl(pos);

    mouse.setX(pos.x());
    mouse.setY(pos.y());
}

void Renderer::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        QPoint pos = event->pos();

        convertPointToOpenGl(pos);

        mouse.setZ(-qAbs(pos.x()));
        mouse.setW(-qAbs(pos.y()));
    }
}

QString Renderer::defaultFragmentShader() const
{
    return QString{
        "#version 330 core\n"
        "\n"
        "out vec4 fragColor;\n"
        "\n"
        "// time (in seconds)\n"
        "uniform float iTime;\n"
        "// shader playback frame\n"
        "uniform int iFrame;\n"
        "// viewport resolution (in pixels)\n"
        "uniform vec2 iResolution;\n"
        "// mouse pixel coords. xy: current (if LMB down), zw: click\n"
        "uniform vec4 iMouse;\n"
        "// input channels\n"
        "uniform sampler2D iChannel0;\n"
        "uniform sampler2D iChannel1;\n"
        "uniform sampler2D iChannel2;\n"
        "uniform sampler2D iChannel3;\n"
        "\n"
        "void main(void)\n"
        "{\n"
        "    // normalized pixel coordinates (from 0 to 1)\n"
        "    vec2 uv = gl_FragCoord.xy / iResolution;\n"
        "\n"
        "    // time varying pixel color\n"
        "    vec3 col = 0.5 + 0.5 * cos(iTime + uv.xyx + vec3(0.0, 2.0, 4.0));\n"
        "\n"
        "    // output to screen\n"
        "    fragColor = vec4(col, 1.0);\n"
        "}\n"
    };
}

void Renderer::createEffect(int index)
{
    Q_ASSERT(!effects.contains(index));

    Effect *effect = createEffect();

    effects[index] = effect;

    // first created effect will become the main image
    if (!mainImage) {
        mainImage = effect;
    }
}

void Renderer::deleteEffect(int index)
{
    Q_ASSERT(effects.contains(index));

    Effect *effect = effects.value(index);

    int removed = effects.remove(index);

    Q_ASSERT(removed == 1);

    // prevent using this effect as other effects inputs before deletion
    removeEffectFromInputs(effect);

    makeCurrent();

    delete effect;

    doneCurrent();
}

QString Renderer::recompileEffectShader(int index, const QString &source)
{
    Q_ASSERT(effects.contains(index));

    Effect *effect = effects.value(index);
    QOpenGLShader *fragment = effect->fragmentShader;
    QString log;

    makeCurrent();

    if (!fragment->compileSourceCode(source.toLocal8Bit().data())) {
        // failed to compile new source code, save log and fallback
        log = fragment->log();

        fragment->compileSourceCode(effect->fallbackSource.toLocal8Bit().data());
    }
    else {
        effect->fallbackSource = source;
    }

    bool result = effect->program->link();

    Q_ASSERT(result == true);

    // reset playback frame counter
    effect->frame = 0;

    doneCurrent();

    return log;
}

void Renderer::effectInputChanged(int index, int channel, int effectIndex)
{
    Q_ASSERT(effects.contains(index));

    Effect *effect = effects.value(index);
    Effect *inputEffect = effects.contains(effectIndex) ?
                            effects.value(effectIndex) : Q_NULLPTR;

    Q_ASSERT(channel >= 0);
    Q_ASSERT(effect->inputs.size() > channel);

    effect->inputs[channel].effect = inputEffect;
}

void Renderer::effectFilteringChanged(int index, int channel, GLint value)
{
    Q_ASSERT(effects.contains(index));

    Effect *effect = effects.value(index);

    Q_ASSERT(channel >= 0);
    Q_ASSERT(effect->inputs.size() > channel);

    effect->inputs[channel].filter = value;
}

void Renderer::effectWrapChanged(int index, int channel, GLint value)
{
    Q_ASSERT(effects.contains(index));

    Effect *effect = effects.value(index);

    Q_ASSERT(channel >= 0);
    Q_ASSERT(effect->inputs.size() > channel);

    effect->inputs[channel].wrap = value;
}

void Renderer::setupVertexShader()
{
    const QString vertexSource{
        "#version 330 core\n"
        "layout (location = 0) in vec2 pos;\n"
        "void main() {\n"
        "gl_Position = vec4(pos, 0.0, 1.0);\n"
        "}\n"
    };

    vertexShader = new QOpenGLShader(QOpenGLShader::ShaderTypeBit::Vertex, this);

    bool result = vertexShader->compileSourceCode(vertexSource);

    Q_ASSERT(result == true);
}

void Renderer::setupBuffers()
{
    bool result = vao.create();

    Q_ASSERT(result == true);

    vao.bind();

    result = vbo.create();

    Q_ASSERT(result == true);

    result = vbo.bind();

    Q_ASSERT(result == true);

    const GLfloat vertices[] = {
        -1.0, 1.0,
        -1.0, -1.0,
        1.0, -1.0,

        -1.0, 1.0,
        1.0, -1.0,
        1.0, 1.0
    };

    vbo.setUsagePattern(QOpenGLBuffer::UsagePattern::StaticDraw);
    vbo.allocate(vertices, sizeof(vertices));

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), Q_NULLPTR);
}

Effect* Renderer::createEffect()
{
    makeCurrent();

    QOpenGLShader *fragment = new QOpenGLShader(QOpenGLShader::ShaderTypeBit::Fragment, this);
    QString source = defaultFragmentShader();

    bool result = fragment->compileSourceCode(source);

    Q_ASSERT(result == true);

    QOpenGLShaderProgram *program = new QOpenGLShaderProgram(this);

    result = program->addShader(vertexShader);

    Q_ASSERT(result == true);

    result = program->addShader(fragment);

    Q_ASSERT(result == true);

    result = program->link();

    Q_ASSERT(result == true);

    QOpenGLFramebufferObject *fbo = new QOpenGLFramebufferObject(fboTextureSize);

    doneCurrent();

    return new Effect(program, fragment, fbo, source);
}

void Renderer::renderEffects()
{
    glViewport(0, 0, fboTextureSize.width(), fboTextureSize.height());

    for (auto &effect : effects) {
        Q_ASSERT(effect != Q_NULLPTR);

        // skip main image rendering
        if (effect == mainImage) {
            continue;
        }

        bool result = effect->framebuffer->bind();
        Q_ASSERT(result == true);

        renderEffect(*effect, fboTextureSize);
        effect->frame++;
    }
}

void Renderer::renderMainImage()
{
    Q_ASSERT(mainImage != Q_NULLPTR);

    // render main image using default fbo
    bool result = QOpenGLFramebufferObject::bindDefault();

    Q_ASSERT(result == true);

    glViewport(0, 0, viewSize.width(), viewSize.height());

    renderEffect(*mainImage, viewSize);
    mainImage->frame++;
}

void Renderer::renderEffect(Effect &effect, QSize textureSize)
{
    Q_UNUSED(textureSize);

    bindEffectTextures(effect);

    auto program = effect.program;

    Q_ASSERT(program != Q_NULLPTR);

    bool result = program->bind();

    Q_ASSERT(result == true);

    setUniforms(effect, textureSize);

    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Renderer::removeEffectFromInputs(const Effect *effect)
{
    for (auto &item : effects) {
        for (auto &input : item->inputs) {
            if (input.effect == effect) {
                input.effect = Q_NULLPTR;
            }
        }
    }
}

void Renderer::bindEffectTextures(const Effect &effect)
{
    int textureUnit = 0;

    for (auto &input : effect.inputs) {
        glActiveTexture(GL_TEXTURE0 + textureUnit);
        textureUnit++;

        auto otherEffect = input.effect;
        // if there is no input effect used, unbind texture
        GLuint id = otherEffect ? otherEffect->framebuffer->texture() : 0;

        glBindTexture(GL_TEXTURE_2D, id);

        if (id) {
            setSamplingParameters(input);
        }
    }
}

void Renderer::setSamplingParameters(const EffectChannelSettings &settings)
{
    switch (settings.filter) {
    case GL_LINEAR_MIPMAP_LINEAR:
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glGenerateMipmap(GL_TEXTURE_2D);
        break;
    case GL_LINEAR:
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        break;
    case GL_NEAREST:
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        break;
    default:
        Q_ASSERT(false);
    }

    switch (settings.wrap) {
    case GL_REPEAT:
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        break;
    case GL_CLAMP_TO_EDGE:
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        break;

    default:
        Q_ASSERT(false);
    }
}

void Renderer::setUniforms(const Effect &effect, QSize textureSize)
{
    QOpenGLShaderProgram *program = effect.program;
    GLfloat currentTime = timer.elapsed() / 1000.0f;

    program->setUniformValue("iTime", currentTime);
    program->setUniformValue("iFrame", effect.frame);
    program->setUniformValue("iResolution", textureSize);
    program->setUniformValue("iMouse", mouse);
    program->setUniformValue("iChannel0", 0);
    program->setUniformValue("iChannel1", 1);
    program->setUniformValue("iChannel2", 2);
    program->setUniformValue("iChannel3", 3);
}

void Renderer::convertPointToOpenGl(QPoint &point) const
{
    // convert Y coordinate to OpenGL: (0, 0) is bottom-left corner
    point.setY(viewSize.height() - point.y());
}
