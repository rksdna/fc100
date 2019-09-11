#ifndef DEVICEPROCESSINGWIDGET_H
#define DEVICEPROCESSINGWIDGET_H

#include <QWidget>

class QSpinBox;
class QComboBox;
class QLineEdit;
class Device;

class DeviceProcessingWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DeviceProcessingWidget(Device *device, QWidget *parent = 0);

    void accept();

private:
    Device * const m_device;
    QComboBox * const m_modeBox;
    QSpinBox * const m_sizeBox;
    QComboBox * const m_conversionBox;
    QLineEdit * const m_expressionEdit;
};

#endif // DEVICEPROCESSINGWIDGET_H
