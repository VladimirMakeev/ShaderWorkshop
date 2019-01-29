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
