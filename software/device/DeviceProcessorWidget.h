#ifndef DEVICEPROCESSORWIDGET_H
#define DEVICEPROCESSORWIDGET_H

#include <QWidget>

class QSpinBox;
class QComboBox;
class QLineEdit;
class DeviceProcessor;
class DeviceProcessorFormatWidget;

class DeviceProcessorWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DeviceProcessorWidget(DeviceProcessor *processor, QWidget *parent = 0);

    void accept();

private:
    DeviceProcessor * const m_processor;
    QList<DeviceProcessorFormatWidget *> m_widgets;
    QComboBox * const m_modeBox;
    QSpinBox * const m_sizeBox;
    QComboBox * const m_conversionBox;
    QLineEdit * const m_expressionEdit;
};

#endif // DEVICEPROCESSORWIDGET_H
