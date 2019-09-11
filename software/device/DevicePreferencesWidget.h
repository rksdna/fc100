#ifndef DEVICEPREFERENCESWIDGET_H
#define DEVICEPREFERENCESWIDGET_H

#include <QWidget>

class Device;
class DeviceFormatBox;
class CustomColorButton;

class DevicePreferencesWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DevicePreferencesWidget(Device *device, QWidget *parent = 0);

    void accept();

private:
    Device * const m_device;
    QList<DeviceFormatBox *> m_widgets;
    CustomColorButton * const m_processorButton;
    CustomColorButton * const m_controllerButton;
};

#endif // DEVICEPREFERENCESWIDGET_H
