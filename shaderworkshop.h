#ifndef SHADERWORKSHOP_H
#define SHADERWORKSHOP_H

#include <QWidget>
#include <QTabWidget>
#include <QComboBox>
#include <QHash>

namespace Ui {
class ShaderWorkshop;
}

class EditorPage;
class Renderer;

class ShaderWorkshop : public QWidget
{
    Q_OBJECT

public:
    explicit ShaderWorkshop(QWidget *parent = 0);
    ~ShaderWorkshop();

protected:
    virtual void paintEvent(QPaintEvent *event);

private slots:
    void newBufferRequested(const QString &name);
    void bufferCloseRequested(int tabIndex);

    void on_actionRecompile_Shader_triggered();

    void on_actionOpen_triggered();

    void on_actionSave_triggered();

private:
    void setupWidgets();
    EditorPage* createPage(const QString &name, int pageIndex);
    void createMenus();
    QString bufferName(int index) const;
    EditorPage* currentPage() const;
    int pageIndex(EditorPage *page) const;

    Ui::ShaderWorkshop *ui;
    Renderer *renderer;
    QTabWidget *tab;
    QComboBox *comboBox;
    QHash<QString, EditorPage*> pages;
    /// indices for renderer effects management
    QHash<EditorPage*, int> pageIndices;
    const QString defaultItemName;
    const int maxBufferPages;
    const int imagePageIndex;
    bool imageEffectCreated;
};

#endif // SHADERWORKSHOP_H
