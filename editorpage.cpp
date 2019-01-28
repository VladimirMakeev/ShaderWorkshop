#include "editorpage.h"
#include "ui_editorpage.h"

EditorPage::EditorPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EditorPage)
{
    ui->setupUi(this);

    ui->listWidget->hide();
}

EditorPage::~EditorPage()
{
    delete ui;
}
