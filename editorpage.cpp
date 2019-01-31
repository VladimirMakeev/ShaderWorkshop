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
    ui->listWidget->hide();

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
