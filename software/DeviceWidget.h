#ifndef DEVICEWIDGET_H
#define DEVICEWIDGET_H

#include <QFrame>

class Device;

class DeviceWidget : public QFrame
{
    Q_OBJECT

public:
    explicit DeviceWidget(Device *device, QWidget *parent = 0);

private:
    void updateDevice();

private:
    Device * const m_device;
};

#endif // DEVICEWIDGET_H
