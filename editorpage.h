#ifndef EDITORPAGE_H
#define EDITORPAGE_H

#include <QWidget>
#include <QListWidget>

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

    void shaderLogUpdated(const QString &log);

private slots:
    void logMessageSelected(QListWidgetItem *item);

private:
    bool parseLogMessage(const QString &message, int &line) const;

    Ui::EditorPage *ui;
    CodeEditor *editor;
    GLSLHighlighter *highlighter;
    QListWidget *logList;
};

#endif // EDITORPAGE_H
