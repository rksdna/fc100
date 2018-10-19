#ifndef DEVICE_H
#define DEVICE_H

#include <QString>
#include <QObject>

class QTimer;
class QSerialPort;

class Device : public QObject
{
    Q_OBJECT

public:
    enum Coupling
    {
        DcCompling,
        AcCoupling
    };

    enum Edge
    {
        Ch1RisingEdge,
        Ch1FallingEdge,
        Ch2RisingEdge,
        Ch2FallingEdge,
    };

    enum Clock
    {
        InternalClock,
        ExternalClock
    };

public:
    static quint16 vendorIdentifier();
    static quint16 productIdentifier();

public:
    explicit Device(QObject *parent = 0);

    void setChannelCoupling(int channel, Coupling coupling);
    Coupling channelCoupling(int channel) const;

    void setChannelThreshold(int channel, double threshold);
    double channelThreshold(int channel) const;

    void setTimerClock(Clock clock);
    Clock timerClock() const;

    void setTimerFrequency(double frequency);
    double getTimerFrequency() const;


    void restart(const QString &name);
    void stop();

private:
    struct Channel
    {
        Channel();

        void setThreshold(double value);
        double threshold() const;

        Coupling coupling;
        quint8 dac;
    };

    struct Timer
    {
        Timer();

        Clock clock;
        double frequency;
    };

private:
    void onReadyRead();
    void onTimeout();
    void read(const QByteArray &data);
    void write(const QByteArray &data);

private:
    static quint8 checksum(const QByteArray &data);

private:
    QSerialPort * const m_port;
    QTimer * const m_timer;

private:
    Channel m_channels[2];
    Timer m_timer1;
};

#endif // DEVICE_H
