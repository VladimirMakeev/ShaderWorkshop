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

    connect(logList, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
            this, SLOT(logMessageSelected(QListWidgetItem*)));
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

bool EditorPage::isShaderSourceModified() const
{
    return editor->document()->isModified();
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

void EditorPage::logMessageSelected(QListWidgetItem *item)
{
    int line = 1;

    if (parseLogMessage(item->text(), line)) {
        editor->highlightLine(line);
        editor->setFocus();
    }
}

bool EditorPage::parseLogMessage(const QString &message, int &line) const
{
    // typical OpenGL shader compilation error message:
    // "ERROR: 0:<line>: <description>"
    QRegularExpression re("(\\w+): \\d+:(\\d+): ([^\n]*)");

    auto match = re.match(message);
    bool matched = match.hasMatch();

    if (matched) {
        line = match.captured(2).toInt();
    }

    return matched;
}
