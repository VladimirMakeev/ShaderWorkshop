/*
 * This file is part of ShaderWorkshop (https://github.com/VladimirMakeev/ShaderWorkshop).
 * Copyright (C) 2019 Vladimir Makeev.
 *
 * ShaderWorkshop is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * ShaderWorkshop is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General Public License
 * along with ShaderWorkshop.  If not, see <https://www.gnu.org/licenses/>.
 */

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
