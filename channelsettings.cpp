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

#include "channelsettings.h"
#include "ui_channelsettings.h"

ChannelSettings::ChannelSettings(const PagesData &data, const QString &name,
                                 QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChannelSettings)
{
    ui->setupUi(this);
    ui->channelName->setText(name);

    ui->inputBox->addItem("No input", -1);

    for (const auto &item : data) {
        ui->inputBox->addItem(item.second, item.first);
    }

    ui->filterBox->addItem("Mipmap", GL_LINEAR_MIPMAP_LINEAR);
    ui->filterBox->addItem("Linear", GL_LINEAR);
    ui->filterBox->addItem("Nearest", GL_NEAREST);

    ui->wrapBox->addItem("Repeat", GL_REPEAT);
    ui->wrapBox->addItem("Clamp", GL_CLAMP_TO_EDGE);

    connect(ui->inputBox, SIGNAL(currentIndexChanged(int)),
            this, SLOT(inputChanged(int)));

    connect(ui->filterBox, SIGNAL(currentIndexChanged(int)),
            this, SLOT(filteringChanged(int)));

    connect(ui->wrapBox, SIGNAL(currentIndexChanged(int)),
            this, SLOT(wrapChanged(int)));
}

ChannelSettings::~ChannelSettings()
{
    delete ui;
}

void ChannelSettings::inputChanged(int index)
{
    int data = ui->inputBox->itemData(index).toInt();

    emit channelInputChanged(data);
}

void ChannelSettings::filteringChanged(int index)
{
    GLint value = ui->filterBox->itemData(index).toInt();

    emit channelFilteringChanged(value);
}

void ChannelSettings::wrapChanged(int index)
{
    GLint value = ui->wrapBox->itemData(index).toInt();

    emit channelWrapChanged(value);
}
