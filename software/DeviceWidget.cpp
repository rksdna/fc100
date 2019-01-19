#include <QTimer>
#include <QSettings>
#include <QGridLayout>
#include "Device.h"
#include "DeviceWidget.h"
#include "DisplayWidget.h"
#include "ChannelWidget.h"
#include "ControlWidget.h"

DeviceWidget::DeviceWidget(Device *device, QWidget *parent)
    : QFrame(parent),
      m_device(device),
      m_computer(new Computer(this)),
      m_displayWidget(new DisplayWidget(tr("Display"))),
      m_channel1Widget(new ChannelWidget(tr("Channel 1"))),
      m_channel2Widget(new ChannelWidget(tr("Channel 2"))),
      m_controlWidget(new ControlWidget(tr("Control"))),
      m_timer(new QTimer(this))
{
    setEnabled(false);

    connect(m_device, &Device::connectionStateChanged, this, &DeviceWidget::setEnabled);
    connect(m_device, &Device::measureFinished, this, &DeviceWidget::finishMeasure);
    connect(m_device, &Device::measureFinished, this, &DeviceWidget::updateWidget);

    connect(m_channel1Widget, &ChannelWidget::optionsChanged, this, &DeviceWidget::updateDevice);
    connect(m_channel2Widget, &ChannelWidget::optionsChanged, this, &DeviceWidget::updateDevice);

    connect(m_controlWidget, &ControlWidget::typeChanged, this, &DeviceWidget::clearMeasure);
    connect(m_controlWidget, &ControlWidget::typeChanged, this, &DeviceWidget::updateDevice);
    connect(m_controlWidget, &ControlWidget::typeChanged, this, &DeviceWidget::updateWidget);
    connect(m_controlWidget, &ControlWidget::functionChanged, this, &DeviceWidget::updateWidget);
    connect(m_controlWidget, &ControlWidget::optionsChanged, this, &DeviceWidget::clearMeasure);
    connect(m_controlWidget, &ControlWidget::optionsChanged, this, &DeviceWidget::updateDevice);
    connect(m_controlWidget, &ControlWidget::optionsChanged, this, &DeviceWidget::updateWidget);
    connect(m_controlWidget, &ControlWidget::startRequested, this, &DeviceWidget::startMeasure);
    connect(m_controlWidget, &ControlWidget::clearRequested, this, &DeviceWidget::clearMeasure);
    connect(m_controlWidget, &ControlWidget::clearRequested, this, &DeviceWidget::updateWidget);

    m_timer->setSingleShot(true);
    connect(m_timer, &QTimer::timeout, this, &DeviceWidget::startMeasureLater);

    QGridLayout * const layout = new QGridLayout(this);
    layout->addWidget(m_displayWidget, 0, 0, 1, 3);
    layout->addWidget(m_channel1Widget, 1, 0);
    layout->addWidget(m_controlWidget, 1, 1);
    layout->addWidget(m_channel2Widget, 1, 2);

    QSettings settings;
    settings.beginGroup("DeviceWidget");

    settings.beginGroup("Channel1");
    m_channel1Widget->restoreFromSettings(settings);
    settings.endGroup();

    settings.beginGroup("Channel2");
    m_channel2Widget->restoreFromSettings(settings);
    settings.endGroup();

    settings.beginGroup("Control");
    m_controlWidget->restoreFromSettings(settings);
    settings.endGroup();

    settings.endGroup();

    if (m_controlWidget->isBurstEnabled())
        startMeasure();
}

void DeviceWidget::hideEvent(QHideEvent *event)
{
    QSettings settings;
    settings.beginGroup("DeviceWidget");

    settings.beginGroup("Channel1");
    m_channel1Widget->saveToSettings(settings);
    settings.endGroup();

    settings.beginGroup("Channel2");
    m_channel2Widget->saveToSettings(settings);
    settings.endGroup();

    settings.beginGroup("Control");
    m_controlWidget->saveToSettings(settings);
    settings.endGroup();

    settings.endGroup();

    QFrame::hideEvent(event);
}

void DeviceWidget::updateWidget()
{
    m_displayWidget->display(m_controlWidget->type(),
                             m_computer->valueOf(m_controlWidget->function()),
                             m_computer->lastTime());
}

void DeviceWidget::updateDevice()
{
    m_device->setCh1Options(m_channel1Widget->options());
    m_device->setCh2Options(m_channel2Widget->options());

    const ControlOptions opt = m_controlWidget->options();
    m_device->setOptions(opt);
    m_timer->setInterval(opt.duration);
}

void DeviceWidget::startMeasure()
{
    m_timer->start();
    m_device->startMeasure();
}

void DeviceWidget::clearMeasure()
{
    m_computer->clear();
    if (m_device->isMeasureStarted())
        startMeasure();
}

void DeviceWidget::finishMeasure(const Sample &sample)
{
    m_computer->process(sample.toValue(m_controlWidget->type()), sample.toValue(Sample::TimeType));
    if (!m_timer->isActive())
        startMeasureLater();
}

void DeviceWidget::startMeasureLater()
{
    if (!m_device->isMeasureStarted() && m_controlWidget->isBurstEnabled())
        startMeasure();
}
