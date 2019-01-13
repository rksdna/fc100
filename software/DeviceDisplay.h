#ifndef DEVICEDISPLAY_H
#define DEVICEDISPLAY_H

#include <QQueue>

class DeviceDisplay
{

public:
    enum Format
    {
        Frequency,
        Period,
        RPM,
        Events,
        Time
    };

public:
    DeviceDisplay();

    void clear();
    void update(int events, qreal time);

    qreal min(Format format) const;
    qreal max(Format format) const;
    qreal average(Format format) const;
    qreal value(Format format) const;

private:
    struct Sample
    {
        Sample(int events = 0, qreal time = 0.0);

        qreal events;
        qreal time;

        qreal toValue(Format format) const;
    };

private:
    QQueue<Sample> m_samples;
};

#endif // DEVICEDISPLAY_H
