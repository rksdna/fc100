#include <QDebug>
#include <QLabel>
#include <QGridLayout>
#include <QFontDatabase>
#include "Device.h"
#include "PopupButton.h"
#include "DeviceSample.h"
#include "DeviceWidget.h"
#include "DeviceChannelWidget.h"
#include "DeviceControlWidget.h"

DeviceWidget::DeviceWidget(Device *device, QWidget *parent)
    : QFrame(parent),
      m_device(device),
      m_valueLabel(new QLabel),
      m_auxLabel(new QLabel),
      m_channel1Widget(new DeviceChannelWidget(tr("Channel 1"))),
      m_channel2Widget(new DeviceChannelWidget(tr("Channel 2"))),
      m_controlWidget(new DeviceControlWidget(tr("Control")))
{
    setEnabled(false);
    connect(m_device, &Device::connectionStateChanged, this, &DeviceWidget::setEnabled);
    connect(m_device, &Device::samplingFinished, this, &DeviceWidget::processSample);

    QFont font = QFontDatabase::systemFont(QFontDatabase::FixedFont);
    const int fontSize = font.pointSize();

    font.setPointSize(3 * fontSize);
    m_valueLabel->setFont(font);
    m_valueLabel->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    m_valueLabel->setAlignment(Qt::AlignVCenter | Qt::AlignRight);

    font.setPointSize(2 * fontSize);
    m_auxLabel->setFont(font);
    m_auxLabel->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    m_auxLabel->setAlignment(Qt::AlignVCenter | Qt::AlignRight);

    connect(m_channel1Widget, &DeviceChannelWidget::deviceChannelChanged, this, &DeviceWidget::updateDevice);

    connect(m_channel2Widget, &DeviceChannelWidget::deviceChannelChanged, this, &DeviceWidget::updateDevice);

    connect(m_controlWidget, &DeviceControlWidget::startRequested, m_device, &Device::startSampling);

    connect(m_controlWidget, &DeviceControlWidget::clearRequested, this, &DeviceWidget::clearFilter);
    connect(m_controlWidget, &DeviceControlWidget::clearRequested, this, &DeviceWidget::updateWidget);

    connect(m_controlWidget, &DeviceControlWidget::modeChanged, this, &DeviceWidget::clearFilter);
    connect(m_controlWidget, &DeviceControlWidget::modeChanged, this, &DeviceWidget::updateDevice);
    connect(m_controlWidget, &DeviceControlWidget::modeChanged, this, &DeviceWidget::updateWidget);

    connect(m_controlWidget, &DeviceControlWidget::viewChanged, this, &DeviceWidget::updateWidget);

    QGridLayout * const layout = new QGridLayout(this);
    layout->addWidget(m_valueLabel, 0, 0, 1, 3);
    layout->addWidget(m_auxLabel, 1, 0, 1, 3);

    layout->addWidget(m_channel1Widget, 2, 0);
    layout->addWidget(m_controlWidget, 2, 1);
    layout->addWidget(m_channel2Widget, 2, 2);

    m_channel1Widget->setProbe(DeviceChannelWidget::x1Probe);
    m_channel1Widget->setDeviceChannel(DeviceChannel());

    m_channel2Widget->setProbe(DeviceChannelWidget::x1Probe);
    m_channel2Widget->setDeviceChannel(DeviceChannel());

    m_controlWidget->setMode(DeviceSample::FrequencyType);
    m_controlWidget->setView(DeviceFilter::LastType);
    m_controlWidget->setBurstEnabled(true);
    m_controlWidget->setDeviceMode(DeviceMode());

    updateDevice();
    m_device->startSampling();
}

void DeviceWidget::clearFilter()
{
    m_filter = DeviceFilter();
}

void DeviceWidget::updateWidget()
{
    const qreal value = m_filter.toValue(m_controlWidget->view());
    switch (m_controlWidget->mode())
    {
    case DeviceSample::FrequencyType:
        m_valueLabel->setText(qIsFinite(value) ? tr("%1 Hz").arg(value, 12, 'f', 3) : tr("--- Hz"));
        m_valueLabel->setVisible(true);

        m_auxLabel->setVisible(false);
        break;

    case DeviceSample::RpmType:
        m_valueLabel->setText(qIsFinite(value) ? tr("%1 RPM").arg(value, 12, 'f', 3) : tr("--- RPM"));
        m_valueLabel->setVisible(true);

        m_auxLabel->setVisible(false);
        break;

    case DeviceSample::PeriodType:
        m_valueLabel->setText(qIsFinite(value) ? tr("%1 S").arg(value, 12, 'f', 9) : tr("--- S"));
        m_valueLabel->setVisible(true);

        m_auxLabel->setVisible(false);
        break;

    case DeviceSample::TimeType:
        m_valueLabel->setText(qIsFinite(value) ? tr("%1 S").arg(value, 12, 'f', 9) : tr("--- S"));
        m_valueLabel->setVisible(true);

        m_auxLabel->setVisible(false);
        break;

    default:
        m_valueLabel->setVisible(false);
        m_auxLabel->setVisible(false);

        break;
    }
}

void DeviceWidget::updateDevice()
{
    m_device->setChannel1(m_channel1Widget->deviceChannel());
    m_device->setChannel2(m_channel2Widget->deviceChannel());
    m_device->setMode(m_controlWidget->fixedDeviceMode());
}

void DeviceWidget::processSample(const DeviceSample &sample)
{
    m_filter.append(sample.toValue(m_controlWidget->mode()));

    updateWidget();

    if (m_controlWidget->isBurstEnabled())
        m_device->startSampling();
}
