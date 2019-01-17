#ifndef DEVICEWIDGET_H
#define DEVICEWIDGET_H

#include <QFrame>

class QLabel;
class QToolButton;
class Device;
class DeviceSample;
class ButtonGroup;

class DeviceWidget : public QFrame
{
    Q_OBJECT

public:
    explicit DeviceWidget(Device *device, QWidget *parent = 0);

private:
    void clearSamples();
    void updateMode();
    void updateWidget();
    void processSample(const DeviceSample &sample);

private:
    struct Samples
    {
        enum Type
        {
            LastType,
            MinValue,
            MaxValue,
            BandValue,
            DeviationValue,
            AverageValue
        };

        Samples();

        void append(qreal value);
        qreal toValue(Type parameter) const;

        qreal first;
        qreal min;
        qreal max;
        qreal average;
        qreal last;
    };

private:
    Device * const m_device;
    QLabel * const m_valueLabel;
    QLabel * const m_timeLabel;
    ButtonGroup * const m_modeButtons;
    ButtonGroup * const m_proButtons;
    QToolButton * const m_runButton;
    QToolButton * const m_clearButton;
    QToolButton * const m_burstButton;
    Samples m_sample;
};

#endif // DEVICEWIDGET_H
