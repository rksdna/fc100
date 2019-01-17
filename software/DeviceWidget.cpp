#include <QDebug>
#include <QLabel>
#include <QGridLayout>
#include <QToolButton>
#include "Device.h"
#include "DeviceSample.h"
#include "DeviceWidget.h"
//#include "DeviceModeWidget.h"
#include "DeviceChannelWidget.h"
#include "ButtonGroup.h"

#include <QFontDatabase>



DeviceWidget::DeviceWidget(Device *device, QWidget *parent)
    : QFrame(parent),
      m_device(device),
      m_valueLabel(new QLabel),
      m_timeLabel(new QLabel),
      m_modeButtons(new ButtonGroup),
      m_triggerButton(new QToolButton),
      m_burstButton(new QToolButton)
{
    setEnabled(false);
    connect(m_device, &Device::connectionStateChanged, this, &DeviceWidget::setEnabled);
    connect(m_device, &Device::samplingFinished, this, &DeviceWidget::processSample);

    QFont font = QFontDatabase::systemFont(QFontDatabase::FixedFont);
    const int fontSize = font.pointSize();

    font.setPointSize(3 * fontSize);
    m_valueLabel->setFont(font);
    m_valueLabel->setAlignment(Qt::AlignVCenter | Qt::AlignRight);

    font.setPointSize(2 * fontSize);
    m_timeLabel->setFont(font);
    m_timeLabel->setAlignment(Qt::AlignVCenter | Qt::AlignRight);

    connect(m_modeButtons, &ButtonGroup::currentDataChanged, this, &DeviceWidget::updateMode);

    m_triggerButton->setText(tr("run"));
    connect(m_triggerButton, &QToolButton::clicked, m_device, &Device::startSampling);

    m_burstButton->setText("Burst");
    m_burstButton->setCheckable(true);

    DeviceChannelWidget * const channel1Widget = new DeviceChannelWidget(tr("Channel 1"));
    connect(channel1Widget, &DeviceChannelWidget::deviceChannelChanged, m_device, &Device::setChannel1);

    DeviceChannelWidget * const channel2Widget = new DeviceChannelWidget(tr("Channel 2"));
    connect(channel2Widget, &DeviceChannelWidget::deviceChannelChanged, m_device, &Device::setChannel2);




    QGridLayout * const layout = new QGridLayout(this);
    layout->addWidget(m_valueLabel, 0, 0, 1, 2);
    layout->addWidget(m_timeLabel, 0, 2, 1, 3);

    layout->addWidget(channel1Widget, 1, 0);
    layout->addWidget(channel2Widget, 1, 2);

    layout->addWidget(m_triggerButton, 2, 0);
    layout->addWidget(m_burstButton, 2, 1);

    layout->addWidget(m_modeButtons->addDataButton(tr("Frequency"), FrequencyMode), 3, 0);
    layout->addWidget(m_modeButtons->addDataButton(tr("Period"), PeriodMode), 3, 1);
    layout->addWidget(m_modeButtons->addDataButton(tr("RPM"), RpmMode), 3, 2);
}

void DeviceWidget::updateMode()
{

}

void DeviceWidget::processSample(const DeviceSample &sample)
{
    qreal value = 0.0;
    bool isValueValid = sample.convert(DeviceSample::FrequencyFormat, value);
    m_valueLabel->setText(isValueValid ? tr("%1 Hz").arg(value, 12, 'f', 3) : tr("--- Hz"));

    qreal time = 0.0;
    bool timeValid = sample.convert(DeviceSample::TimeFormat, time);
    m_timeLabel->setText(timeValid ? tr("%1 S").arg(time, 12, 'f', 3) : tr("--- S"));

    if (m_burstButton->isChecked())
        m_device->startSampling();
}
