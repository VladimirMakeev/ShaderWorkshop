#include "editorpage.h"
#include "glslhighlighter.h"
#include "ui_editorpage.h"

EditorPage::EditorPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EditorPage)
{
    ui->setupUi(this);

    ui->listWidget->hide();

    highlighter = new GLSLHighlighter(ui->plainTextEdit->document());
}

EditorPage::~EditorPage()
{
    delete ui;
}
