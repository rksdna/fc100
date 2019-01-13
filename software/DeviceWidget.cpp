#include <QDebug>
#include <QGridLayout>
#include <QToolButton>
#include "Device.h"
#include "DeviceSample.h"
#include "DeviceWidget.h"
#include "DeviceModeWidget.h"
#include "DeviceDisplayWidget.h"
#include "DeviceChannelWidget.h"

DeviceWidget::DeviceWidget(Device *device, QWidget *parent)
    : QFrame(parent),
      m_device(device),
      m_triggerButton(new QToolButton),
      m_burstButton(new QToolButton)
{
    setEnabled(false);
    connect(m_device, &Device::connectionStateChanged, this, &DeviceWidget::setEnabled);
    connect(m_device, &Device::samplingFinished, this, &DeviceWidget::processSample);

    m_triggerButton->setText(tr("run"));
    connect(m_triggerButton, &QToolButton::clicked, m_device, &Device::startSampling);

    m_burstButton->setText("Burst");
    m_burstButton->setCheckable(true);

    DeviceDisplayWidget * const display = new DeviceDisplayWidget(device->display());

    DeviceChannelWidget * const channel1Widget = new DeviceChannelWidget(tr("Channel 1"));
    connect(channel1Widget, &DeviceChannelWidget::deviceChannelChanged, m_device, &Device::setChannel1);

    DeviceChannelWidget * const channel2Widget = new DeviceChannelWidget(tr("Channel 2"));
    connect(channel2Widget, &DeviceChannelWidget::deviceChannelChanged, m_device, &Device::setChannel2);

    DeviceModeWidget * const modeWidget = new DeviceModeWidget("Mode", tr("Mode"), device->mode());

    QGridLayout * const layout = new QGridLayout(this);
    layout->addWidget(display, 0, 0, 1, 3);

    layout->addWidget(channel1Widget, 1, 0);
    layout->addWidget(modeWidget, 1, 1);
    layout->addWidget(channel2Widget, 1, 2);

    layout->addWidget(m_triggerButton, 2, 0);
    layout->addWidget(m_burstButton, 2, 1);
}

void DeviceWidget::processSample(const DeviceSample &sample)
{
    if (sample.valid)
        qDebug().noquote() << "F =" << sample.counter / sample.timer;
    else
        qDebug() << "---";

    if (m_burstButton->isChecked())
        m_device->startSampling();
}
