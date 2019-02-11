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

#include "shaderworkshop.h"
#include "renderer.h"
#include "ui_shaderworkshop.h"
#include <QMenuBar>
#include <QFileDialog>
#include <QMessageBox>

ShaderWorkshop::ShaderWorkshop(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ShaderWorkshop),
    imagePage(Q_NULLPTR),
    defaultItemName("Add buffer"),
    maxBufferPages(5),
    imagePageIndex(0),
    imageEffectCreated(false)
{
    ui->setupUi(this);

    renderer = ui->openGLWidget;

    setupWidgets();
    createMenus();
}

ShaderWorkshop::~ShaderWorkshop()
{
    qDeleteAll(pages);
    delete ui;
}

void ShaderWorkshop::paintEvent(QPaintEvent *event)
{
    if (!imageEffectCreated) {
        // create effect for main image page right before painting.
        // By doing it here we can be sure that Renderer already initialized
        // its OpenGL context
        renderer->createEffect(imagePageIndex);

        connectPage(imagePage);
        imageEffectCreated = true;
    }

    QWidget::paintEvent(event);
}

void ShaderWorkshop::newBufferRequested(const QString &name)
{
    // default combo box item activations should not create new buffers
    if (name == defaultItemName) {
        return;
    }

    Q_ASSERT(pages.contains(name));

    EditorPage *page = pages.value(name);
    page->setShaderSource(renderer->defaultFragmentShader());
    page->clearShaderLog();

    tab->insertTab(tab->count(), page, name);
    comboBox->removeItem(comboBox->findText(name));
    // always show default combo box item
    comboBox->setCurrentText(defaultItemName);

    renderer->createEffect(pageIndex(page));

    connectPage(page);
}

void ShaderWorkshop::bufferCloseRequested(int tabIndex)
{
    QString name = tab->tabText(tabIndex);

    Q_ASSERT(pages.contains(name));

    tab->removeTab(tabIndex);
    comboBox->addItem(name);

    EditorPage *page = pages.value(name);

    renderer->deleteEffect(pageIndex(page));

    disconnectPage(page);
}

void ShaderWorkshop::setupWidgets()
{
    tab = ui->tabWidget;
    tab->setTabsClosable(true);

    Q_ASSERT(tab->count() == 0);

    comboBox = ui->comboBox;
    comboBox->addItem(defaultItemName);

    PagesData pagesData;
    int pageIndex = imagePageIndex + 1;

    for (int i = 1; i < maxBufferPages; i++, pageIndex++) {
        pagesData.append(QPair<int, QString>(pageIndex, bufferName(i - 1)));
    }

    createImagePage(pagesData);

    createPages(pagesData);

    connect(comboBox, static_cast<void (QComboBox::*)(const QString&)>(&QComboBox::activated),
            this, &ShaderWorkshop::newBufferRequested);

    connect(tab, SIGNAL(tabCloseRequested(int)),
            this, SLOT(bufferCloseRequested(int)));
}

EditorPage* ShaderWorkshop::createPage(const QString &name, int pageIndex,
                                       const PagesData &data)
{
    EditorPage *page = new EditorPage(pageIndex, data);

    pages[name] = page;
    pageIndices[page] = pageIndex;

    return page;
}

void ShaderWorkshop::createImagePage(const PagesData &data)
{
    imagePage = createPage("Image", imagePageIndex, data);
    imagePage->setShaderSource(renderer->defaultFragmentShader());

    int tabIndex = tab->insertTab(tab->count(), imagePage, tr("Image"));
    // user should not be able to close main image page
    tab->tabBar()->setTabButton(tabIndex, QTabBar::LeftSide, Q_NULLPTR);
}

void ShaderWorkshop::createPages(const PagesData &data)
{
    for (const auto &item : data) {
        const QString &name = item.second;

        createPage(name, item.first, data);
        comboBox->addItem(name);
    }
}

void ShaderWorkshop::createMenus()
{
    QMenuBar *bar = new QMenuBar(this);
    QMenu *file = bar->addMenu(tr("&File"));
    QMenu *build = bar->addMenu(tr("&Build"));
    QMenu *about = bar->addMenu(tr("&Help"));

    file->addAction(ui->actionOpen);
    file->addAction(ui->actionSave);
    build->addAction(ui->actionRecompile_Shader);
    about->addAction(ui->actionAbout);
}

QString ShaderWorkshop::bufferName(int index) const
{
    char letter = 'A' + index;

    return QString("Buffer %1").arg(letter);
}

EditorPage* ShaderWorkshop::currentPage() const
{
    return static_cast<EditorPage*>(tab->currentWidget());
}

int ShaderWorkshop::pageIndex(EditorPage *page) const
{
    Q_ASSERT(pageIndices.contains(page));

    return pageIndices.value(page);
}

void ShaderWorkshop::connectPage(EditorPage *page)
{
    connect(page, SIGNAL(channelInputChanged(int,int,int)),
            renderer, SLOT(effectInputChanged(int,int,int)));

    connect(page, SIGNAL(channelFilteringChanged(int,int,GLint)),
            renderer, SLOT(effectFilteringChanged(int,int,GLint)));

    connect(page, SIGNAL(channelWrapChanged(int,int,GLint)),
            renderer, SLOT(effectWrapChanged(int,int,GLint)));
}

void ShaderWorkshop::disconnectPage(EditorPage *page)
{
    disconnect(page, SIGNAL(channelInputChanged(int,int,int)),
               renderer, SLOT(effectInputChanged(int,int,int)));

    disconnect(page, SIGNAL(channelFilteringChanged(int,int,GLint)),
               renderer, SLOT(effectFilteringChanged(int,int,GLint)));

    disconnect(page, SIGNAL(channelWrapChanged(int,int,GLint)),
               renderer, SLOT(effectWrapChanged(int,int,GLint)));
}

void ShaderWorkshop::on_actionRecompile_Shader_triggered()
{
    EditorPage *page = currentPage();
    const QString source = page->shaderSource();

    QString log = renderer->recompileEffectShader(pageIndex(page), source);

    page->shaderLogUpdated(log);
}

void ShaderWorkshop::on_actionOpen_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Shader"), "",
                        tr("GLSL Fragment shader (*.frag);; Text file (*.txt)"));

    if (fileName.isEmpty()) {
        return;
    }

    QFile file(fileName);

    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::critical(this, tr("Error"),
                              tr("Could not open file %1:\n%2")
                                .arg(fileName)
                                .arg(file.errorString()));
        return;
    }

    currentPage()->setShaderSource(file.readAll());
}

void ShaderWorkshop::on_actionSave_triggered()
{
    const EditorPage *page = currentPage();

    if (!page->isShaderSourceModified()) {
        return;
    }

    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Shader"), "",
                        tr("GLSL Fragment shader (*.frag);; Text file (*.txt)"));

    if (fileName.isEmpty()) {
        return;
    }

    QFile file(fileName);

    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Shader Workshop"),
                             tr("Could not write file %1:\n%2")
                                .arg(fileName)
                                .arg(file.errorString()));
        return;
    }

    QTextStream out(&file);

    out << page->shaderSource();
}

void ShaderWorkshop::on_actionAbout_triggered()
{
    const QString text{
        "<h3>ShaderWorkshop</h3>\n\n"
        "Inspired by <a href='https://www.shadertoy.com'>ShaderToy</a>, "
        "made with <a href='https://www.qt.io'>Qt</a>\n\n"
        "<p>Copyright (C) 2019 Vladimir Makeev.\n"
        "ShaderWorkshop is licensed under the GNU LGPL version 3.0.\n"
        "Source code is available at "
        "<a href='https://github.com/VladimirMakeev/ShaderWorkshop'>GitHub</a>.</p>"
    };

    QMessageBox box(this);
    box.setTextFormat(Qt::RichText);
    box.setWindowTitle("About ShaderWorkshop");
    box.setText(text);
    box.exec();
}
