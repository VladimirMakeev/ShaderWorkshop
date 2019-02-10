#ifndef EDITORPAGE_H
#define EDITORPAGE_H

#include <QWidget>
#include <QListWidget>
#include <QList>
#include <QPair>
#include <QOpenGLFunctions>

namespace Ui {
class EditorPage;
}

class GLSLHighlighter;
class CodeEditor;
class ChannelSettings;

using PagesData = QList<QPair<int, QString>>;

class EditorPage : public QWidget
{
    Q_OBJECT

public:
    EditorPage(int pageIndex, const PagesData &data, QWidget *parent = Q_NULLPTR);
    ~EditorPage();

    QString shaderSource() const;
    void setShaderSource(const QString &source);
    bool isShaderSourceModified() const;

    void shaderLogUpdated(const QString &log);
    void clearShaderLog();

signals:
    void channelInputChanged(int pageIndex, int channelNumber, int newPageIndex);
    void channelFilteringChanged(int pageIndex, int channelNumber, GLint value);
    void channelWrapChanged(int pageIndex, int channelNumber, GLint value);

private slots:
    void logMessageSelected(QListWidgetItem *item);

    void onChannelInputSettingChanged(int newPageIndex);
    void onChannelFilteringChanged(GLint value);
    void onChannelWrapChanged(GLint value);

private:
    void setupChannelSettings(const PagesData &data);
    bool parseLogMessage(const QString &message, int &line) const;
    int channelNumber(ChannelSettings *channel) const;

    Ui::EditorPage *ui;
    CodeEditor *editor;
    GLSLHighlighter *highlighter;
    QListWidget *logList;
    QList<ChannelSettings*> channels;
    int pageIndex;
};

#endif // EDITORPAGE_H
