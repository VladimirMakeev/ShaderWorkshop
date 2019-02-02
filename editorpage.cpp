#include "editorpage.h"
#include "glslhighlighter.h"
#include "codeeditor.h"
#include "ui_editorpage.h"

EditorPage::EditorPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EditorPage)
{
    ui->setupUi(this);

    editor = ui->plainTextEdit;
    logList = ui->listWidget;
    logList->hide();

    highlighter = new GLSLHighlighter(editor->document());
}

EditorPage::~EditorPage()
{
    delete ui;
}

QString EditorPage::shaderSource() const
{
    return editor->toPlainText();
}

void EditorPage::setShaderSource(const QString &source)
{
    editor->setPlainText(source);
}

void EditorPage::shaderLogUpdated(const QString &log)
{
    logList->clear();

    if (log.isEmpty()) {
        logList->hide();
        return;
    }

    QString::SplitBehavior behavior = QString::SplitBehavior::SkipEmptyParts;
    logList->addItems(log.split('\n', behavior));
    logList->show();
}
