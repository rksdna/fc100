#include <QDial>
#include <QGridLayout>
#include "PopupButton.h"
#include "ControlWidget.h"

ControlWidget::ControlWidget(const QString &title, QWidget *parent)
    : QGroupBox(title, parent),
      m_typeButton(new PopupButton),
      m_functionButton(new PopupButton),
      m_startEdgeButton(new PopupButton),
      m_stopEdgeButton(new PopupButton),
      m_counterEdgeButton(new PopupButton),
      m_timerClockButton(new PopupButton),
      m_durationDial(new QDial),
      m_startButton(new QToolButton),
      m_burstButton(new QToolButton),
      m_clearButton(new QToolButton)
{
    m_typeButton->addData(tr("Frequency"), Sample::FrequencyType);
    m_typeButton->addData(tr("Period"), Sample::PeriodType);
    m_typeButton->addData(tr("RPM"), Sample::RpmType);
    m_typeButton->addData(tr("Time"), Sample::TimeType);
    connect(m_typeButton, &PopupButton::currentDataChanged, this, &ControlWidget::updateWidget);
    connect(m_typeButton, &PopupButton::currentDataChanged, this, &ControlWidget::typeChanged);

    m_functionButton->addData(tr("Last"), Computer::ActualFunction);
    m_functionButton->addData(tr("Min"), Computer::MinFunction);
    m_functionButton->addData(tr("Max"), Computer::MaxFunction);
    m_functionButton->addData(tr("Band"), Computer::BandFunction);
    m_functionButton->addData(tr("Deviation"), Computer::DeviationFunction);
    m_functionButton->addData(tr("Average"), Computer::AverageFunction);
    connect(m_functionButton, &PopupButton::currentDataChanged, this, &ControlWidget::functionChanged);

    m_startEdgeButton->addData(tr("1R"), ControlOptions::Ch1RisingEvent);
    m_startEdgeButton->addData(tr("1F"), ControlOptions::Ch1FallingEvent);
    m_startEdgeButton->addData(tr("2R"), ControlOptions::Ch2RisingEvent);
    m_startEdgeButton->addData(tr("2F"), ControlOptions::Ch2FallingEvent);
    connect(m_startEdgeButton, &PopupButton::currentDataChanged, this, &ControlWidget::typeChanged);

    m_stopEdgeButton->addData(tr("1R"), ControlOptions::Ch1RisingEvent);
    m_stopEdgeButton->addData(tr("1F"), ControlOptions::Ch1FallingEvent);
    m_stopEdgeButton->addData(tr("2R"), ControlOptions::Ch2RisingEvent);
    m_stopEdgeButton->addData(tr("2F"), ControlOptions::Ch2FallingEvent);
    connect(m_stopEdgeButton, &PopupButton::currentDataChanged, this, &ControlWidget::typeChanged);

    m_counterEdgeButton->addData(tr("1R"), ControlOptions::Ch1RisingEvent);
    m_counterEdgeButton->addData(tr("1F"), ControlOptions::Ch1FallingEvent);
    m_counterEdgeButton->addData(tr("2R"), ControlOptions::Ch2RisingEvent);
    m_counterEdgeButton->addData(tr("2F"), ControlOptions::Ch2FallingEvent);
    connect(m_counterEdgeButton, &PopupButton::currentDataChanged, this, &ControlWidget::typeChanged);

    m_timerClockButton->addData(tr("INT"), ControlOptions::InternalClock);
    m_timerClockButton->addData(tr("EXT"), ControlOptions::ExternalClock);
    connect(m_timerClockButton, &PopupButton::currentDataChanged, this, &ControlWidget::typeChanged);

    m_durationDial->setRange(0, 1000);

    m_startButton->setText(tr("Start"));
    connect(m_startButton, &QToolButton::clicked, this, &ControlWidget::startRequested);

    m_burstButton->setText("Burst");
    m_burstButton->setCheckable(true);

    m_clearButton->setText(tr("Clear"));
    connect(m_clearButton, &QToolButton::clicked, this, &ControlWidget::clearRequested);

    QGridLayout * const layout = new QGridLayout(this);
    layout->addWidget(m_functionButton, 0, 1);
    layout->addWidget(m_typeButton, 0, 0);
    layout->addWidget(m_startEdgeButton, 1, 0);
    layout->addWidget(m_stopEdgeButton, 1, 1);
    layout->addWidget(m_counterEdgeButton, 2, 0);
    layout->addWidget(m_timerClockButton, 2, 1);
    layout->addWidget(m_startButton, 0, 2);
    layout->addWidget(m_burstButton, 1, 2);
    layout->addWidget(m_clearButton, 2, 2);
}

Sample::Type ControlWidget::type() const
{
    return Sample::Type(m_typeButton->currentData().toInt());
}

void ControlWidget::setType(Sample::Type type)
{
    m_typeButton->setCurrentData(type);
    updateWidget();
}

Computer::Function ControlWidget::function() const
{
    return Computer::Function(m_functionButton->currentData().toInt());
}

void ControlWidget::setFunction(Computer::Function function)
{
    m_functionButton->setCurrentData(function);
}

ControlOptions ControlWidget::options() const
{
    return ControlOptions(false,
                          ControlOptions::Edge(m_startEdgeButton->currentData().toInt()),
                          ControlOptions::Edge(m_stopEdgeButton->currentData().toInt()),
                          ControlOptions::Edge(m_counterEdgeButton->currentData().toInt()),
                          ControlOptions::Clock(m_timerClockButton->currentData().toInt()),
                          m_durationDial->value());
}

ControlOptions ControlWidget::fixedOptions() const
{
    switch (type())
    {
    case Sample::FrequencyType:
    case Sample::RpmType:
    case Sample::PeriodType:
        return ControlOptions(true,
                              ControlOptions::Edge(m_counterEdgeButton->currentData().toInt()),
                              ControlOptions::Edge(m_counterEdgeButton->currentData().toInt()),
                              ControlOptions::Edge(m_counterEdgeButton->currentData().toInt()),
                              ControlOptions::Clock(m_timerClockButton->currentData().toInt()),
                              m_durationDial->value());

    case Sample::TimeType:
        return ControlOptions(false,
                              ControlOptions::Edge(m_startEdgeButton->currentData().toInt()),
                              ControlOptions::Edge(m_stopEdgeButton->currentData().toInt()),
                              ControlOptions::Edge(m_counterEdgeButton->currentData().toInt()),
                              ControlOptions::Clock(m_timerClockButton->currentData().toInt()),
                              m_durationDial->value());

    default:
        break;
    }

    return ControlOptions();
}

void ControlWidget::setOptions(const ControlOptions &options)
{
    m_startEdgeButton->setCurrentData(options.startEdge);
    m_stopEdgeButton->setCurrentData(options.stopEdge);
    m_counterEdgeButton->setCurrentData(options.counterEgde);
    m_timerClockButton->setCurrentData(options.timerClock);
    m_durationDial->setValue(options.duration);
}

bool ControlWidget::isBurstEnabled() const
{
    return m_burstButton->isChecked();
}

void ControlWidget::setBurstEnabled(bool enabled)
{
    m_burstButton->setChecked(enabled);
}

void ControlWidget::updateWidget()
{
    switch (type())
    {
    case Sample::FrequencyType:
    case Sample::RpmType:
    case Sample::PeriodType:
        m_startEdgeButton->setEnabled(false);
        m_stopEdgeButton->setEnabled(false);
        m_counterEdgeButton->setEnabled(true);
        break;

    case Sample::TimeType:
        m_startEdgeButton->setEnabled(true);
        m_stopEdgeButton->setEnabled(true);
        m_counterEdgeButton->setEnabled(false);
        break;

    default:
        break;
    }
}
