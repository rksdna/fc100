#ifndef DEVICEWIDGET_H
#define DEVICEWIDGET_H

#include <QFrame>
#include "Computer.h"

class Device;
class Sample;
class DisplayWidget;
class ChannelWidget;
class ControlWidget;

class DeviceWidget : public QFrame
{
    Q_OBJECT

public:
    explicit DeviceWidget(Device *device, QWidget *parent = 0);

private:
    void clearFilter();
    void updateWidget();
    void updateDevice();
    void processSample(const Sample &sample);

private:
    Device * const m_device;
    DisplayWidget * const m_displayWidget;
    ChannelWidget * const m_channel1Widget;
    ChannelWidget * const m_channel2Widget;
    ControlWidget * const m_controlWidget;
    Computer m_filter;
};

#endif // DEVICEWIDGET_H
