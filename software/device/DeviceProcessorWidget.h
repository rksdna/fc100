#ifndef DEVICEPROCESSORWIDGET_H
#define DEVICEPROCESSORWIDGET_H

#include <QWidget>

class DeviceProcessor;

class DeviceProcessorWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DeviceProcessorWidget(DeviceProcessor *processor, QWidget *parent = 0);

    void accept();

private:
    DeviceProcessor * const m_processor;
};

#endif // DEVICEPROCESSORWIDGET_H
