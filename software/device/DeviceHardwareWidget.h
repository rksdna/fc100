#ifndef DEVICEHARDWAREWIDGET_H
#define DEVICEHARDWAREWIDGET_H

#include <QWidget>

class QComboBox;
class QDoubleSpinBox;
class Device;

class DeviceHardwareWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DeviceHardwareWidget(Device *device, QWidget *parent = 0);

    void accept();

private:
    Device * const m_device;
    QComboBox * const m_portBox;
    QComboBox * const m_sourceBox;
    QDoubleSpinBox * const m_frequencyBox;
};

#endif // DEVICEHARDWAREWIDGET_H
