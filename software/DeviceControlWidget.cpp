#include <QDial>
#include <QGridLayout>
#include "PopupButton.h"
#include "DeviceControlWidget.h"

DeviceControlWidget::DeviceControlWidget(const QString &title, QWidget *parent)
    : QGroupBox(title, parent),
      m_startEdgeButton(new PopupButton),
      m_stopEdgeButton(new PopupButton),
      m_counterEdgeButton(new PopupButton),
      m_timerClockButton(new PopupButton),
      m_durationDial(new QDial),
      m_modeButton(new PopupButton),
      m_viewButton(new PopupButton),
      m_startButton(new QToolButton),
      m_clearButton(new QToolButton),
      m_burstButton(new QToolButton)
{
    m_modeButton->addData(tr("Frequency"), DeviceSample::FrequencyType);
    m_modeButton->addData(tr("Period"), DeviceSample::PeriodType);
    m_modeButton->addData(tr("RPM"), DeviceSample::RpmType);
    m_modeButton->addData(tr("Time"), DeviceSample::TimeType);
    connect(m_modeButton, &PopupButton::currentDataChanged, this, &DeviceControlWidget::updateWidget);
    connect(m_modeButton, &PopupButton::currentDataChanged, this, &DeviceControlWidget::modeChanged);

    m_viewButton->addData(tr("Last"), DeviceFilter::LastType);
    m_viewButton->addData(tr("Min"), DeviceFilter::MinValue);
    m_viewButton->addData(tr("Max"), DeviceFilter::MaxValue);
    m_viewButton->addData(tr("Band"), DeviceFilter::BandValue);
    m_viewButton->addData(tr("Deviation"), DeviceFilter::DeviationValue);
    m_viewButton->addData(tr("Average"), DeviceFilter::AverageValue);
    connect(m_viewButton, &PopupButton::currentDataChanged, this, &DeviceControlWidget::viewChanged);

    m_startEdgeButton->addData(tr("1R"), DeviceMode::Ch1RisingEvent);
    m_startEdgeButton->addData(tr("1F"), DeviceMode::Ch1FallingEvent);
    m_startEdgeButton->addData(tr("2R"), DeviceMode::Ch2RisingEvent);
    m_startEdgeButton->addData(tr("2F"), DeviceMode::Ch2FallingEvent);
    connect(m_startEdgeButton, &PopupButton::currentDataChanged, this, &DeviceControlWidget::modeChanged);

    m_stopEdgeButton->addData(tr("1R"), DeviceMode::Ch1RisingEvent);
    m_stopEdgeButton->addData(tr("1F"), DeviceMode::Ch1FallingEvent);
    m_stopEdgeButton->addData(tr("2R"), DeviceMode::Ch2RisingEvent);
    m_stopEdgeButton->addData(tr("2F"), DeviceMode::Ch2FallingEvent);
    connect(m_stopEdgeButton, &PopupButton::currentDataChanged, this, &DeviceControlWidget::modeChanged);

    m_counterEdgeButton->addData(tr("1R"), DeviceMode::Ch1RisingEvent);
    m_counterEdgeButton->addData(tr("1F"), DeviceMode::Ch1FallingEvent);
    m_counterEdgeButton->addData(tr("2R"), DeviceMode::Ch2RisingEvent);
    m_counterEdgeButton->addData(tr("2F"), DeviceMode::Ch2FallingEvent);
    connect(m_counterEdgeButton, &PopupButton::currentDataChanged, this, &DeviceControlWidget::modeChanged);

    m_timerClockButton->addData(tr("INT"), DeviceMode::InternalClock);
    m_timerClockButton->addData(tr("EXT"), DeviceMode::ExternalClock);
    connect(m_timerClockButton, &PopupButton::currentDataChanged, this, &DeviceControlWidget::modeChanged);

    m_startButton->setText(tr("Start"));
    connect(m_startButton, &QToolButton::clicked, this, &DeviceControlWidget::startRequested);

    m_clearButton->setText(tr("Clear"));
    connect(m_clearButton, &QToolButton::clicked, this, &DeviceControlWidget::clearRequested);

    m_burstButton->setText("Burst");
    m_burstButton->setCheckable(true);

    QGridLayout * const layout = new QGridLayout(this);

    layout->addWidget(m_startButton, 0, 0);
    layout->addWidget(m_burstButton, 0, 1);
    layout->addWidget(m_clearButton, 0, 2);

    layout->addWidget(m_modeButton, 1, 0);
    layout->addWidget(m_viewButton, 1, 1);

    layout->addWidget(m_startEdgeButton, 2, 0);
    layout->addWidget(m_stopEdgeButton, 2, 1);
    layout->addWidget(m_counterEdgeButton, 2, 2);
    layout->addWidget(m_timerClockButton, 2, 3);
}

DeviceSample::Type DeviceControlWidget::mode() const
{
    return DeviceSample::Type(m_modeButton->currentData().toInt());
}

void DeviceControlWidget::setMode(DeviceSample::Type mode)
{
    m_modeButton->setCurrentData(mode);
    updateWidget();
}

DeviceFilter::Type DeviceControlWidget::view() const
{
    return DeviceFilter::Type(m_viewButton->currentData().toInt());
}

void DeviceControlWidget::setView(DeviceFilter::Type view)
{
    m_viewButton->setCurrentData(view);
}

bool DeviceControlWidget::isBurstEnabled() const
{
    return m_burstButton->isChecked();
}

void DeviceControlWidget::setBurstEnabled(bool enabled)
{
    m_burstButton->setChecked(enabled);
}

DeviceMode DeviceControlWidget::deviceMode() const
{
    return DeviceMode(DeviceMode::Edge(m_startEdgeButton->currentData().toInt()),
                      DeviceMode::Edge(m_stopEdgeButton->currentData().toInt()),
                      DeviceMode::Edge(m_counterEdgeButton->currentData().toInt()),
                      DeviceMode::Clock(m_timerClockButton->currentData().toInt()),
                      m_durationDial->value());
}

DeviceMode DeviceControlWidget::fixedDeviceMode() const
{
    switch (mode())
    {
    case DeviceSample::FrequencyType:
    case DeviceSample::RpmType:
    case DeviceSample::PeriodType:
        return DeviceMode(DeviceMode::Edge(m_counterEdgeButton->currentData().toInt()),
                          DeviceMode::Edge(m_counterEdgeButton->currentData().toInt()),
                          DeviceMode::Edge(m_counterEdgeButton->currentData().toInt()),
                          DeviceMode::Clock(m_timerClockButton->currentData().toInt()),
                          m_durationDial->value());

    case DeviceSample::TimeType:
        return DeviceMode(DeviceMode::Edge(m_startEdgeButton->currentData().toInt()),
                          DeviceMode::Edge(m_stopEdgeButton->currentData().toInt()),
                          DeviceMode::Edge(m_counterEdgeButton->currentData().toInt()),
                          DeviceMode::Clock(m_timerClockButton->currentData().toInt()),
                          m_durationDial->value());

    default:
        break;
    }

    return DeviceMode();
}

void DeviceControlWidget::setDeviceMode(const DeviceMode &mode)
{
    m_startEdgeButton->setCurrentData(mode.startEdge);
    m_stopEdgeButton->setCurrentData(mode.stopEdge);
    m_counterEdgeButton->setCurrentData(mode.counterEgde);
    m_timerClockButton->setCurrentData(mode.timerClock);
    m_durationDial->setValue(mode.duration);
}

void DeviceControlWidget::updateWidget()
{
    switch (mode())
    {
    case DeviceSample::FrequencyType:
    case DeviceSample::RpmType:
    case DeviceSample::PeriodType:
        m_startEdgeButton->setEnabled(false);
        m_stopEdgeButton->setEnabled(false);
        m_counterEdgeButton->setEnabled(true);
        break;

    case DeviceSample::TimeType:
        m_startEdgeButton->setEnabled(true);
        m_stopEdgeButton->setEnabled(true);
        m_counterEdgeButton->setEnabled(false);
        break;

    default:
        break;
    }
}
