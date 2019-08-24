#ifndef DEVICEDIALOG_H
#define DEVICEDIALOG_H

#include <QDialog>

class QSpinBox;
class QComboBox;
class QLineEdit;
class QDoubleSpinBox;
class Device;

class DeviceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DeviceDialog(Device *device, QWidget *parent = 0);

    void accept();

protected:
    void hideEvent(QHideEvent *event);

private:
    Device * const m_device;
    QComboBox * const m_clockBox;
    QDoubleSpinBox * const m_clockFrequencyBox;
    QComboBox * const m_timeUnitBox;
    QSpinBox * const m_timeDecimalsBox;
    QComboBox * const m_frequencyUnitBox;
    QSpinBox * const m_frequencyDecimalsBox;
    QLineEdit * const m_functionEdit;
    QLineEdit * const m_functionUnitEdit;
    QSpinBox * const m_functionDecimalsBox;
    QComboBox * const m_portNameBox;
};

#endif // DEVICEDIALOG_H

