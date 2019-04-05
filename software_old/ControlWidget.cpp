#include <QSettings>
#include <QGridLayout>
#include "Dial.h"
#include "ToolButton.h"
#include "PopupButton.h"
#include "ControlWidget.h"

ControlWidget::ControlWidget(const QString &title, QWidget *parent)
    : QGroupBox( parent),
      m_title(title),
      m_typeButton(new PopupButton),
      m_functionButton(new PopupButton),
      m_startEdgeButton(new PopupButton),
      m_stopEdgeButton(new PopupButton),
      m_counterEdgeButton(new PopupButton),
      m_timerClockButton(new PopupButton),
      m_durationDial(new Dial),
      m_startButton(new ToolButton),
      m_burstButton(new PopupButton),
      m_gatedButton(new PopupButton),
      m_clearButton(new ToolButton)
{
    int key = -1;
    m_durations.insert(++key, qMakePair(10, tr("10 mS")));
    m_durations.insert(++key, qMakePair(50, tr("50 mS")));
    m_durations.insert(++key, qMakePair(100, tr("100 mS")));
    m_durations.insert(++key, qMakePair(500, tr("500 mS")));
    m_durations.insert(++key, qMakePair(1000, tr("1 S")));
    m_durations.insert(++key, qMakePair(5000, tr("5 S")));
    m_durations.insert(++key, qMakePair(10000, tr("10 S")));

    m_typeButton->appendData(tr("Frequency"), Sample::FrequencyType);
    m_typeButton->appendData(tr("Period"), Sample::PeriodType);
    m_typeButton->appendData(tr("RPM"), Sample::RpmType);
    m_typeButton->appendData(tr("Count"), Sample::CountType);
    m_typeButton->appendData(tr("Time"), Sample::TimeType);
    connect(m_typeButton, &PopupButton::currentDataChanged, this, &ControlWidget::updateWidget);
    connect(m_typeButton, &PopupButton::currentDataChanged, this, &ControlWidget::typeChanged);

    m_functionButton->appendData(tr("Actual"), Computer::ActualFunction);
    m_functionButton->appendData(tr("Smooth"), Computer::SmoothFunction);
    m_functionButton->appendData(tr("Min"), Computer::MinFunction);
    m_functionButton->appendData(tr("Max"), Computer::MaxFunction);
    m_functionButton->appendData(tr("Band"), Computer::BandFunction);
    m_functionButton->appendData(tr("Deviation"), Computer::DeviationFunction);
    connect(m_functionButton, &PopupButton::currentDataChanged, this, &ControlWidget::functionChanged);

    m_startEdgeButton->appendData(tr("Ch.1 Rise"), ControlOptions::Ch1RisingEvent);
    m_startEdgeButton->appendData(tr("Ch.1 fall"), ControlOptions::Ch1FallingEvent);
    m_startEdgeButton->appendData(tr("Ch.2 Rise"), ControlOptions::Ch2RisingEvent);
    m_startEdgeButton->appendData(tr("Ch.2 Fall"), ControlOptions::Ch2FallingEvent);
    connect(m_startEdgeButton, &PopupButton::currentDataChanged, this, &ControlWidget::optionsChanged);

    m_stopEdgeButton->appendData(tr("Ch.1 Rise"), ControlOptions::Ch1RisingEvent);
    m_stopEdgeButton->appendData(tr("Ch.1 Fall"), ControlOptions::Ch1FallingEvent);
    m_stopEdgeButton->appendData(tr("Ch.2 Rise"), ControlOptions::Ch2RisingEvent);
    m_stopEdgeButton->appendData(tr("Ch.2 Fall"), ControlOptions::Ch2FallingEvent);
    connect(m_stopEdgeButton, &PopupButton::currentDataChanged, this, &ControlWidget::optionsChanged);

    m_counterEdgeButton->appendData(tr("Ch.1 Rise"), ControlOptions::Ch1RisingEvent);
    m_counterEdgeButton->appendData(tr("Ch.1 Fall"), ControlOptions::Ch1FallingEvent);
    m_counterEdgeButton->appendData(tr("Ch.2 Rise"), ControlOptions::Ch2RisingEvent);
    m_counterEdgeButton->appendData(tr("Ch.2 Fall"), ControlOptions::Ch2FallingEvent);
    connect(m_counterEdgeButton, &PopupButton::currentDataChanged, this, &ControlWidget::optionsChanged);

    m_timerClockButton->appendData(tr("Int. Ref."), ControlOptions::InternalClock);
    m_timerClockButton->appendData(tr("Ext. Ref."), ControlOptions::ExternalClock);
    connect(m_timerClockButton, &PopupButton::currentDataChanged, this, &ControlWidget::optionsChanged);

    m_durationDial->setRange(0, key);
    connect(m_durationDial, &QDial::valueChanged, this, &ControlWidget::optionsChanged);
    connect(m_durationDial, &QDial::valueChanged, this, &ControlWidget::updateWidget);

    m_startButton->setText(tr("Start"));
    connect(m_startButton, &QToolButton::clicked, this, &ControlWidget::startRequested);

    m_burstButton->appendData(tr("Auto"), true);
    m_burstButton->appendData(tr("Manual"), false);

    m_gatedButton->appendData(tr("Free"), false);
    m_gatedButton->appendData(tr("Gated"), true);
    connect(m_gatedButton, &PopupButton::currentDataChanged, this, &ControlWidget::updateWidget);
    connect(m_gatedButton, &PopupButton::currentDataChanged, this, &ControlWidget::optionsChanged);

    m_clearButton->setText(tr("Clear"));
    connect(m_clearButton, &QToolButton::clicked, this, &ControlWidget::clearRequested);

    QGridLayout * const layout = new QGridLayout(this);
    layout->addWidget(m_functionButton, 0, 0);
    layout->addWidget(m_gatedButton, 0, 1);
    layout->addWidget(m_typeButton, 0, 2);
    layout->addWidget(m_counterEdgeButton, 0, 3);
    layout->addWidget(m_timerClockButton, 0, 4);

    layout->addWidget(m_burstButton, 1, 0);
    layout->addWidget(m_startButton, 1, 1);
    layout->addWidget(m_clearButton, 1, 2);
    layout->addWidget(m_startEdgeButton, 1, 3);
    layout->addWidget(m_stopEdgeButton, 1, 4);

    layout->addWidget(m_durationDial, 0, 5, 2, 2);
}

Sample::Type ControlWidget::type() const
{
    return Sample::Type(m_typeButton->currentData().toInt());
}

Computer::Function ControlWidget::function() const
{
    return Computer::Function(m_functionButton->currentData().toInt());
}

ControlOptions ControlWidget::options() const
{
    const bool isTimeMode = type() == Sample::TimeType;
    const bool isGatedMode = isTimeMode || m_gatedButton->currentData().toBool();
    return ControlOptions(!isGatedMode,
                          ControlOptions::Edge((isGatedMode ? m_startEdgeButton : m_counterEdgeButton)->currentData().toInt()),
                          ControlOptions::Edge((isGatedMode ? m_stopEdgeButton : m_counterEdgeButton)->currentData().toInt()),
                          ControlOptions::Edge(m_counterEdgeButton->currentData().toInt()),
                          ControlOptions::Clock(m_timerClockButton->currentData().toInt()),
                          m_durations.value(m_durationDial->value()).first);
}

bool ControlWidget::isBurstEnabled() const
{
    return m_burstButton->currentData().toBool();
}

void ControlWidget::saveToSettings(QSettings &settings)
{
    settings.setValue("Type", m_typeButton->currentData());
    settings.setValue("Function", m_functionButton->currentData());
    settings.setValue("Start", m_startEdgeButton->currentData());
    settings.setValue("Stop", m_stopEdgeButton->currentData());
    settings.setValue("Counter", m_counterEdgeButton->currentData());
    settings.setValue("Timer", m_timerClockButton->currentData());
    settings.setValue("Duration", m_durationDial->value());
    settings.setValue("Burst", m_burstButton->currentData());
    settings.setValue("Gate", m_gatedButton->currentData());
}

void ControlWidget::restoreFromSettings(const QSettings &settings)
{
    m_typeButton->setCurrentData(settings.value("Type", Sample::FrequencyType));
    m_functionButton->setCurrentData(settings.value("Function", ControlOptions::Ch1RisingEvent));
    m_startEdgeButton->setCurrentData(settings.value("Start", ControlOptions::Ch1RisingEvent));
    m_stopEdgeButton->setCurrentData(settings.value("Stop", ControlOptions::Ch1RisingEvent));
    m_counterEdgeButton->setCurrentData(settings.value("Counter", ControlOptions::Ch1RisingEvent));
    m_timerClockButton->setCurrentData(settings.value("Timer", ControlOptions::InternalClock));
    m_durationDial->setValue(settings.value("Duration", 0).toInt());
    m_burstButton->setCurrentData(settings.value("Burst", true));
    m_gatedButton->setCurrentData(settings.value("Gate", false));
}

void ControlWidget::updateWidget()
{
    const bool isTimeMode = type() == Sample::TimeType;
    const bool isGatedMode = isTimeMode || m_gatedButton->currentData().toBool();
    m_startEdgeButton->setEnabled(isGatedMode);
    m_stopEdgeButton->setEnabled(isGatedMode);
    m_counterEdgeButton->setEnabled(!isTimeMode);
    m_gatedButton->setEnabled(!isTimeMode);

    const int key = m_durationDial->value();
    setTitle(tr("%1: %2").arg(m_title).arg(m_durations.value(key).second));
}
