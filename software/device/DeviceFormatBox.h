#ifndef DEVICEFORMATBOX_H
#define DEVICEFORMATBOX_H

#include <QGroupBox>
#include "DeviceProcessor.h"

class QSpinBox;
class QComboBox;

class DeviceFormatBox : public QGroupBox
{
    Q_OBJECT

public:
    explicit DeviceFormatBox(const QString &title, DeviceProcessor *processor, DeviceProcessor::Type type, QWidget *parent = 0);

    void accept();

private:
    DeviceProcessor * const m_processor;
    const DeviceProcessor::Type m_type;
    QComboBox * const m_unitBox;
    QSpinBox * const m_decimalsBox;
};

#endif // DEVICEFORMATBOX_H
