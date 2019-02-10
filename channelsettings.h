#ifndef CHANNELSETTINGS_H
#define CHANNELSETTINGS_H

#include <QWidget>
#include <QOpenGLFunctions>
#include "editorpage.h"

namespace Ui {
class ChannelSettings;
}

class ChannelSettings : public QWidget
{
    Q_OBJECT

public:
    explicit ChannelSettings(const PagesData &data, const QString &name,
                             QWidget *parent = Q_NULLPTR);
    ~ChannelSettings();

signals:
    void channelInputChanged(int pageIndex);
    void channelFilteringChanged(GLint value);
    void channelWrapChanged(GLint value);

private slots:
    void inputChanged(int index);
    void filteringChanged(int index);
    void wrapChanged(int index);

private:
    Ui::ChannelSettings *ui;
};

#endif // CHANNELSETTINGS_H
