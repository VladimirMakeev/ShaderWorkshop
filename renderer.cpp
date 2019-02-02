#include "renderer.h"

Renderer::Renderer(QWidget *parent) :
    QOpenGLWidget(parent),
    mainImage(Q_NULLPTR),
    fboTextureSize(1024, 768)
{
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

QString Renderer::defaultFragmentShader() const
{
    return QString{
        "#version 330 core\n"
        "\n"
        "out vec4 fragColor;\n"
        "\n"
        "void main(void) {\n"
        "fragColor = vec4(0.0, 1.0, 0.5, 1.0);\n"
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

    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Renderer::bindEffectTextures(const Effect &effect)
{
    int textureUnit = 0;

    for (auto &input : effect.inputs) {
        glActiveTexture(GL_TEXTURE0 + textureUnit);

        auto otherEffect = input.effect;
        // if there is no input effect used, unbind texture
        GLuint id = otherEffect ? otherEffect->framebuffer->texture() : 0;

        glBindTexture(GL_TEXTURE_2D, id);
    }
}
