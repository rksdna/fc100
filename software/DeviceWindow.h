#ifndef DEVICEWINDOW_H
#define DEVICEWINDOW_H

#include <QWidget>

class Device;

class DeviceWindow : public QWidget
{
    Q_OBJECT

public:
    explicit DeviceWindow(QWidget *parent = 0);

private:
    Device * const m_device;
};

#endif // DEVICEWINDOW_H
