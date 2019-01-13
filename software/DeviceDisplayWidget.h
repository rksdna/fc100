#ifndef DEVICEDISPLAYWIDGET_H
#define DEVICEDISPLAYWIDGET_H

#include <QWidget>

class QLabel;
class DeviceDisplay;

class DeviceDisplayWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DeviceDisplayWidget(DeviceDisplay *display, QWidget *parent = 0);

private:
    void updateDeviceDisplayWidget();

private:
    DeviceDisplay * const m_display;
    QLabel * const m_firstLabel;
    QLabel * const m_secondLabel;
};

#endif // DEVICEDISPLAYWIDGET_H
