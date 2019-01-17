#ifndef DEVICEWIDGET_H
#define DEVICEWIDGET_H

#include <QFrame>

class QLabel;
class QToolButton;
class Device;
class DeviceSample;
class ButtonGroup;

class DeviceWidget : public QFrame
{
    Q_OBJECT

public:
    enum Mode
    {
        FrequencyMode,
        PeriodMode,
        RpmMode
    };

public:
    explicit DeviceWidget(Device *device, QWidget *parent = 0);

private:
    void updateMode();
    void processSample(const DeviceSample &sample);

private:
    Device * const m_device;
    QLabel * const m_valueLabel;
    QLabel * const m_timeLabel;
    ButtonGroup * const m_modeButtons;
    QToolButton * const m_triggerButton;
    QToolButton * const m_burstButton;
};

#endif // DEVICEWIDGET_H
