#ifndef EDITORPAGE_H
#define EDITORPAGE_H

#include <QWidget>

namespace Ui {
class EditorPage;
}

class GLSLHighlighter;
class CodeEditor;

class EditorPage : public QWidget
{
    Q_OBJECT

public:
    explicit EditorPage(QWidget *parent = 0);
    ~EditorPage();

    QString shaderSource() const;
    void setShaderSource(const QString &source);

private:
    Ui::EditorPage *ui;
    CodeEditor *editor;
    GLSLHighlighter *highlighter;
};

#endif // EDITORPAGE_H
