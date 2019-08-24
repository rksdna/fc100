#ifndef DEVICECHANNEL_H
#define DEVICECHANNEL_H

#include <QObject>

class QSettings;

class DeviceChannel : public QObject
{
    Q_OBJECT

public:
    enum Coupling
    {
        DcCoupling,
        AcCoupling
    };

    Q_ENUM(Coupling)

    enum Probe
    {
        x1Probe,
        x10Probe,
        x100Probe
    };

    Q_ENUM(Probe)

public:
    static int minThreshold();
    static int maxThreshold();

public:
    explicit DeviceChannel(QObject *parent = 0);

    Coupling coupling() const;
    void setCoupling(Coupling coupling);

    Probe probe() const;
    void setProbe(Probe probe);

    int threshold() const;
    void setThreshold(int threshold);

    QString text() const;

    void saveToSettings(QSettings &settings);
    void restoreFromSettings(QSettings &settings);

signals:
    void couplingChanged(Coupling coupling);
    void probeChanged(Probe probe);
    void thresholdChanged(int threshold);
    void textChanged(const QString &text);

private:
    Coupling m_coupling;
    Probe m_probe;
    int m_threshold;
};

#endif // DEVICECHANNEL_H
