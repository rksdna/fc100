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

    QComboBox * const m_referenceSourceBox;
    QDoubleSpinBox * const m_referenceFrequencyBox;

    /*FormatWidget * const m_countFormatWidget;
    FormatWidget * const m_timeFormatWidget;
    FormatWidget * const m_frequencyFormatWidget;
    FormatWidget * const m_userFormatWidget;*/


    QLineEdit * const m_functionEdit;

    QComboBox * const m_portNameBox;
};

#endif // DEVICEDIALOG_H

