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
