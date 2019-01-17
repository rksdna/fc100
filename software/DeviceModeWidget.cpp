#include <QSettings>
#include <QGridLayout>
#include "PopupButton.h"
#include "DeviceMode.h"
#include "DeviceModeWidget.h"

/*DeviceModeWidget::DeviceModeWidget(const QString &tag, const QString &title, DeviceMode *mode, QWidget *parent)
    : QGroupBox(title, parent),
      m_tag(tag),
      m_mode(mode),
      m_startButton(new PopupButton),
      m_stopButton(new PopupButton),
      m_counterButton(new PopupButton),
      m_timerButton(new PopupButton),
      m_durationButton(new PopupButton)
{
    setAlignment(Qt::AlignRight);

    m_startButton->appendItem(tr("B1⭜"), DeviceMode::Ch1RisingEvent);
    m_startButton->appendItem(tr("B1⮯"), DeviceMode::Ch1FallingEvent);
    m_startButton->appendItem(tr("B2⮭"), DeviceMode::Ch2RisingEvent);
    m_startButton->appendItem(tr("B2⮯"), DeviceMode::Ch2FallingEvent);
    connect(m_startButton, &PopupButton::dataChanged, this, &DeviceModeWidget::updateDeviceMode);

    m_stopButton->appendItem(tr("E1⮭"), DeviceMode::Ch1RisingEvent);
    m_stopButton->appendItem(tr("E1⮯"), DeviceMode::Ch1FallingEvent);
    m_stopButton->appendItem(tr("E2⮭"), DeviceMode::Ch2RisingEvent);
    m_stopButton->appendItem(tr("E2⮯"), DeviceMode::Ch2FallingEvent);
    connect(m_stopButton, &PopupButton::dataChanged, this, &DeviceModeWidget::updateDeviceMode);

    m_counterButton->appendItem(tr("C1⮭"), DeviceMode::Ch1RisingEvent);
    m_counterButton->appendItem(tr("C1⮯"), DeviceMode::Ch1FallingEvent);
    m_counterButton->appendItem(tr("C2⮭"), DeviceMode::Ch2RisingEvent);
    m_counterButton->appendItem(tr("C2⮯"), DeviceMode::Ch2FallingEvent);
    connect(m_counterButton, &PopupButton::dataChanged, this, &DeviceModeWidget::updateDeviceMode);

    m_timerButton->appendItem(tr("INT"),  DeviceMode::InternalClock);
    m_timerButton->appendItem(tr("EXT"),  DeviceMode::ExternalClock);
    connect(m_timerButton, &PopupButton::dataChanged, this, &DeviceModeWidget::updateDeviceMode);

    m_durationButton->appendItem(tr("-0-"), 0);
    m_durationButton->appendItem(tr("1 ms"), 1);
    m_durationButton->appendItem(tr("10 ms"), 10);
    m_durationButton->appendItem(tr("50 ms"), 50);
    m_durationButton->appendItem(tr("100 ms"), 100);
    m_durationButton->appendItem(tr("1 S"), 1000);
    m_durationButton->appendItem(tr("5 S"), 5000);
    m_durationButton->appendItem(tr("10 S"), 10000);
    connect(m_durationButton, &PopupButton::dataChanged, this, &DeviceModeWidget::updateDeviceMode);

    QGridLayout * const layout = new QGridLayout(this);
    layout->addWidget(m_startButton, 0, 0);
    layout->addWidget(m_stopButton, 0, 1);
    layout->addWidget(m_counterButton, 0, 2);
    layout->addWidget(m_durationButton, 1, 0, 1, 2);
    layout->addWidget(m_timerButton, 1, 2);

    QSettings settings;
    settings.beginGroup(m_tag);
    m_startButton->setData(settings.value("Start"));
    m_stopButton->setData(settings.value("Stop"));
    m_counterButton->setData(settings.value("Counter"));
    m_timerButton->setData(settings.value("Timer"));
    m_durationButton->setData(settings.value("Duration"));

    updateDeviceMode();
}

DeviceModeWidget::~DeviceModeWidget()
{
    QSettings settings;
    settings.beginGroup(m_tag);
    settings.setValue("Start", m_startButton->data());
    settings.setValue("Stop", m_stopButton->data());
    settings.setValue("Counter", m_counterButton->data());
    settings.setValue("Timer", m_timerButton->data());
    settings.setValue("Duration", m_durationButton->data());
}

void DeviceModeWidget::updateDeviceMode()
{
    m_mode->setStartEvent(DeviceMode::Event(m_startButton->data().toInt()));
    m_mode->setStopEvent(DeviceMode::Event(m_stopButton->data().toInt()));
    m_mode->setCounterEvent(DeviceMode::Event(m_counterButton->data().toInt()));
    m_mode->setTimerClock(DeviceMode::Clock(m_timerButton->data().toInt()));
    m_mode->setDuration(m_durationButton->data().toInt());
}

*/
