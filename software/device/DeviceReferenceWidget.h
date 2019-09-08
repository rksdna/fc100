#ifndef DEVICEREFERENCEWIDGET_H
#define DEVICEREFERENCEWIDGET_H

#include <QWidget>

class QComboBox;
class QDoubleSpinBox;
class DeviceReference;

class DeviceReferenceWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DeviceReferenceWidget(DeviceReference *reference, QWidget *parent = 0);

    void accept();

private:
    DeviceReference * const m_reference;
    QComboBox * const m_sourceBox;
    QDoubleSpinBox * const m_frequencyBox;
};

#endif // DEVICEREFERENCEWIDGET_H
