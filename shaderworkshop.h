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

class ShaderWorkshop : public QWidget
{
    Q_OBJECT

public:
    explicit ShaderWorkshop(QWidget *parent = 0);
    ~ShaderWorkshop();

private slots:
    void newBufferRequested(const QString &name);
    void bufferCloseRequested(int index);

private:
    void setupWidgets();
    QString bufferName(int index) const;

    Ui::ShaderWorkshop *ui;
    QTabWidget *tab;
    QComboBox *comboBox;
    EditorPage *imagePage;
    QHash<QString, EditorPage*> bufferPages;
    const QString defaultItemName;
    const int maxBufferPages;
};

#endif // SHADERWORKSHOP_H
