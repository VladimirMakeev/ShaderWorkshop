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

#ifndef LINENUMBERAREA_H
#define LINENUMBERAREA_H

#include "codeeditor.h"
#include <QWidget>

class LineNumberArea : public QWidget
{
public:
    explicit LineNumberArea(CodeEditor *editor = Q_NULLPTR) :
        QWidget(editor),
        editor(editor)
    {
    }

    QSize sizeHint() const Q_DECL_OVERRIDE
    {
        return QSize(editor->lineNumberAreaWidth(), 0);
    }

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE
    {
        editor->lineNumberAreaPaintEvent(event);
    }

private:
    CodeEditor *editor;
};

#endif // LINENUMBERAREA_H
