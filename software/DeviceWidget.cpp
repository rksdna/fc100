#include <QDebug>
#include <QLabel>
#include <QGridLayout>
#include <QToolButton>
#include <QFontDatabase>
#include "Device.h"
#include "ButtonGroup.h"
#include "DeviceSample.h"
#include "DeviceWidget.h"
#include "DeviceChannelWidget.h"

DeviceWidget::Samples::Samples()
    : first(qQNaN()),
      min(qQNaN()),
      max(qQNaN()),
      average(qQNaN()),
      last(qQNaN())
{
}

void DeviceWidget::Samples::append(qreal value)
{
    if (qIsFinite(value))
    {
        const qreal factor = 0.75;
        first = qIsFinite(first) ? first : value;
        min = qIsFinite(min) ? qMin(min, value) : value;
        max = qIsFinite(max) ? qMax(max, value) : value;
        average = qIsFinite(average) ? factor * average + (1.0 - factor) * value : value;
    }

    last = value;
}

qreal DeviceWidget::Samples::toValue(DeviceWidget::Samples::Type parameter) const
{
    switch (parameter)
    {
    case LastType:
        return last;

    case MinValue:
        return min;

    case MaxValue:
        return max;

    case BandValue:
        return max - min;

    case DeviationValue:
        return last - first;

    case AverageValue:
        return average;
    }

    return qQNaN();
}

DeviceWidget::DeviceWidget(Device *device, QWidget *parent)
    : QFrame(parent),
      m_device(device),
      m_valueLabel(new QLabel),
      m_timeLabel(new QLabel),
      m_modeButtons(new ButtonGroup),
      m_proButtons(new ButtonGroup),
      m_runButton(new QToolButton),
      m_clearButton(new QToolButton),
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

    connect(m_modeButtons, &ButtonGroup::currentDataChanged, this, &DeviceWidget::clearSamples);
    connect(m_proButtons, &ButtonGroup::currentDataChanged, this, &DeviceWidget::updateWidget);

    m_runButton->setText(tr("Run"));
    connect(m_runButton, &QToolButton::clicked, m_device, &Device::startSampling);

    m_clearButton->setText(tr("Clear"));
    connect(m_clearButton, &QToolButton::clicked, this, &DeviceWidget::clearSamples);

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

    layout->addWidget(m_runButton, 2, 0);
    layout->addWidget(m_burstButton, 2, 1);
    layout->addWidget(m_clearButton, 2, 0);

    layout->addWidget(m_modeButtons->addDataButton(tr("Frequency"), DeviceSample::FrequencyType), 3, 0);
    layout->addWidget(m_modeButtons->addDataButton(tr("Period"), DeviceSample::PeriodType), 3, 1);
    layout->addWidget(m_modeButtons->addDataButton(tr("RPM"), DeviceSample::RpmType), 3, 2);

    layout->addWidget(m_proButtons->addDataButton(tr("Last"), Samples::LastType), 4, 0);
    layout->addWidget(m_proButtons->addDataButton(tr("Min"), Samples::MinValue), 4, 1);
    layout->addWidget(m_proButtons->addDataButton(tr("Max"), Samples::MaxValue), 4, 2);
    layout->addWidget(m_proButtons->addDataButton(tr("Band"), Samples::BandValue), 4, 3);
    layout->addWidget(m_proButtons->addDataButton(tr("Deviation"), Samples::DeviationValue), 4, 4);
    layout->addWidget(m_proButtons->addDataButton(tr("Average"), Samples::AverageValue), 4, 5);

    m_burstButton->toggle();
    m_modeButtons->setCurrentData(DeviceSample::FrequencyType);
    m_proButtons->setCurrentData(Samples::LastType);

    processSample(DeviceSample());
}

void DeviceWidget::clearSamples()
{
    m_sample = Samples();

    updateWidget();
}

void DeviceWidget::updateWidget()
{
    const DeviceSample::Type type = DeviceSample::Type(m_modeButtons->currentData().toInt());
    const Samples::Type m2 = Samples::Type(m_proButtons->currentData().toInt());

    const qreal value = m_sample.toValue(m2);
    switch (type)
    {
    case DeviceSample::FrequencyType:
        m_valueLabel->setText(qIsFinite(value) ? tr("%1 Hz").arg(value, 12, 'f', 3) : tr("--- Hz"));
        m_valueLabel->setVisible(true);
        break;

    case DeviceSample::RpmType:
        m_valueLabel->setText(qIsFinite(value) ? tr("%1 RPM").arg(value, 12, 'f', 3) : tr("--- RPM"));
        m_valueLabel->setVisible(true);
        break;

    case DeviceSample::PeriodType:
        m_valueLabel->setText(qIsFinite(value) ? tr("%1 S").arg(value, 12, 'f', 9) : tr("--- S"));
        m_valueLabel->setVisible(true);
        break;

    default:
        m_valueLabel->setVisible(false);
        m_timeLabel->setVisible(false);
        break;
    }
}

void DeviceWidget::processSample(const DeviceSample &sample)
{
    const DeviceSample::Type type = DeviceSample::Type(m_modeButtons->currentData().toInt());
    m_sample.append(sample.toValue(type));

    updateWidget();

    if (m_burstButton->isChecked())
        m_device->startSampling();
}
