#ifndef DEVICEWIDGET_H
#define DEVICEWIDGET_H

#include <QFrame>
#include "DeviceFilter.h"

class QLabel;
class Device;
class PopupButton;
class DeviceSample;
class DeviceChannelWidget;
class DeviceControlWidget;

class DeviceWidget : public QFrame
{
    Q_OBJECT

public:
    explicit DeviceWidget(Device *device, QWidget *parent = 0);

private:
    void clearFilter();
    void updateWidget();
    void updateDevice();
    void processSample(const DeviceSample &sample);

private:
    Device * const m_device;
    QLabel * const m_valueLabel;
    QLabel * const m_auxLabel;
    DeviceChannelWidget * const m_channel1Widget;
    DeviceChannelWidget * const m_channel2Widget;
    DeviceControlWidget * const m_controlWidget;
    DeviceFilter m_filter;
};

#endif // DEVICEWIDGET_H
