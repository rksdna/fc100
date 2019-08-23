#ifndef DEVICEWIDGET_H
#define DEVICEWIDGET_H

#include <QWidget>

class Device;

class DeviceWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DeviceWidget(Device *device, QWidget *parent = 0);
};

#endif // DEVICEWIDGET_H
