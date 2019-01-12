#ifndef DEVICECHANNEL_H
#define DEVICECHANNEL_H

#include <QString>

class DeviceChannel
{
public:
    enum Coupling
    {
        DcCoupling,
        AcCoupling
    };

public:
    DeviceChannel(int min, int max, qreal minVoltage, qreal maxVoltage);

    Coupling coupling() const;
    void setCoupling(Coupling value);

    int minThreshold() const;
    int maxThreshold() const;
    int threshold() const;
    void setThreshold(int value);

    qreal thresholdVoltage() const;

private:
    const int m_min;
    const int m_max;
    const qreal m_minVoltage;
    const qreal m_maxVoltage;
    Coupling m_coupling;
    int m_threshold;
};

#endif // DEVICECHANNEL_H
