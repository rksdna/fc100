#include <QGridLayout>
#include "Device.h"
#include "Sample.h"
#include "DeviceWidget.h"
#include "DisplayWidget.h"
#include "ChannelWidget.h"
#include "ControlWidget.h"

DeviceWidget::DeviceWidget(Device *device, QWidget *parent)
    : QFrame(parent),
      m_device(device),
      m_displayWidget(new DisplayWidget(tr("Display"))),
      m_channel1Widget(new ChannelWidget(tr("Channel 1"))),
      m_channel2Widget(new ChannelWidget(tr("Channel 2"))),
      m_controlWidget(new ControlWidget(tr("Control")))
{
    setEnabled(false);
    connect(m_device, &Device::connectionStateChanged, this, &DeviceWidget::setEnabled);
    connect(m_device, &Device::samplingFinished, this, &DeviceWidget::processSample);

    connect(m_channel1Widget, &ChannelWidget::optionsChanged, this, &DeviceWidget::updateDevice);

    connect(m_channel2Widget, &ChannelWidget::optionsChanged, this, &DeviceWidget::updateDevice);

    connect(m_controlWidget, &ControlWidget::startRequested, m_device, &Device::startSampling);

    connect(m_controlWidget, &ControlWidget::clearRequested, this, &DeviceWidget::clearFilter);
    connect(m_controlWidget, &ControlWidget::clearRequested, this, &DeviceWidget::updateWidget);

    connect(m_controlWidget, &ControlWidget::typeChanged, this, &DeviceWidget::clearFilter);
    connect(m_controlWidget, &ControlWidget::typeChanged, this, &DeviceWidget::updateDevice);
    connect(m_controlWidget, &ControlWidget::typeChanged, this, &DeviceWidget::updateWidget);

    connect(m_controlWidget, &ControlWidget::functionChanged, this, &DeviceWidget::updateWidget);

    QGridLayout * const layout = new QGridLayout(this);
    layout->addWidget(m_displayWidget, 0, 0, 1, 3);
    layout->addWidget(m_channel1Widget, 1, 0);
    layout->addWidget(m_controlWidget, 1, 1);
    layout->addWidget(m_channel2Widget, 1, 2);

    m_channel1Widget->setProbe(ChannelWidget::x1Probe);
    m_channel1Widget->setOptions(ChannelOptions());

    m_channel2Widget->setProbe(ChannelWidget::x1Probe);
    m_channel2Widget->setOptions(ChannelOptions());

    m_controlWidget->setType(Sample::FrequencyType);
    m_controlWidget->setFunction(Computer::ActualFunction);
    m_controlWidget->setBurstEnabled(true);
    m_controlWidget->setOptions(ControlOptions());

    updateDevice();
    m_device->startSampling();
}

void DeviceWidget::clearFilter()
{
    m_filter = Computer();
}

void DeviceWidget::updateWidget()
{
    m_displayWidget->display(m_controlWidget->type(),
                             m_filter.toValue(m_controlWidget->function()),
                             0);
}

void DeviceWidget::updateDevice()
{
    m_device->setCh1Options(m_channel1Widget->options());
    m_device->setCh2Options(m_channel2Widget->options());
    m_device->setOptions(m_controlWidget->fixedOptions());
}

void DeviceWidget::processSample(const Sample &sample)
{
    m_filter.process(sample.toValue(m_controlWidget->type()));

    updateWidget();

    if (m_controlWidget->isBurstEnabled())
        m_device->startSampling();
}
