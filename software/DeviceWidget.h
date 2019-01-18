#ifndef DEVICEWIDGET_H
#define DEVICEWIDGET_H

#include <QFrame>
#include "Computer.h"

class Device;
class Sample;
class Computer;
class DisplayWidget;
class ChannelWidget;
class ControlWidget;

class DeviceWidget : public QFrame
{
    Q_OBJECT

public:
    explicit DeviceWidget(Device *device, QWidget *parent = 0);

protected:
    void hideEvent(QHideEvent *event);

private:
    void updateWidget();
    void updateDevice();
    void startMeasure();
    void clearMeasure();
    void finishMeasure(const Sample &sample);
    void startMeasureLater();

private:
    Device * const m_device;
    Computer * const m_computer;
    DisplayWidget * const m_displayWidget;
    ChannelWidget * const m_channel1Widget;
    ChannelWidget * const m_channel2Widget;
    ControlWidget * const m_controlWidget;
    QTimer * const m_timer;
};

#endif // DEVICEWIDGET_H
