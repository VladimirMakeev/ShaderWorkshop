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

#ifndef SHADERWORKSHOP_H
#define SHADERWORKSHOP_H

#include <QWidget>
#include <QTabWidget>
#include <QComboBox>
#include <QHash>
#include "editorpage.h"

namespace Ui {
class ShaderWorkshop;
}

class EditorPage;
class Renderer;

class ShaderWorkshop : public QWidget
{
    Q_OBJECT

public:
    explicit ShaderWorkshop(QWidget *parent = 0);
    ~ShaderWorkshop();

protected:
    virtual void paintEvent(QPaintEvent *event);

private slots:
    void newBufferRequested(const QString &name);
    void bufferCloseRequested(int tabIndex);

    void on_actionRecompile_Shader_triggered();

    void on_actionOpen_triggered();

    void on_actionSave_triggered();

    void on_actionAbout_triggered();

private:
    void setupWidgets();
    EditorPage* createPage(const QString &name, int pageIndex, const PagesData &data);
    void createImagePage(const PagesData &data);
    void createPages(const PagesData &data);
    void createMenus();
    QString bufferName(int index) const;
    EditorPage* currentPage() const;
    int pageIndex(EditorPage *page) const;
    void connectPage(EditorPage *page);
    void disconnectPage(EditorPage *page);

    Ui::ShaderWorkshop *ui;
    Renderer *renderer;
    QTabWidget *tab;
    QComboBox *comboBox;
    EditorPage *imagePage;
    QHash<QString, EditorPage*> pages;
    /// indices for renderer effects management
    QHash<EditorPage*, int> pageIndices;
    const QString defaultItemName;
    const int maxBufferPages;
    const int imagePageIndex;
    bool imageEffectCreated;
};

#endif // SHADERWORKSHOP_H
