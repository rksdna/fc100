#ifndef DEVICEWIDGET_H
#define DEVICEWIDGET_H

#include <QFrame>

class QLabel;
class QToolButton;
class Device;
class DeviceSample;

class DeviceWidget : public QFrame
{
    Q_OBJECT

public:
    explicit DeviceWidget(Device *device, QWidget *parent = 0);

private:
    void processSample(const DeviceSample &sample);

private:
    Device * const m_device;
    QLabel * const m_valueLabel;
    QLabel * const m_timeLabel;
    QToolButton * const m_triggerButton;
    QToolButton * const m_burstButton;
};

#endif // DEVICEWIDGET_H
