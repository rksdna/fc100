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
        DcCoupling,
        AcCoupling
    };

    enum Edge
    {
        Ch1RisingEdge,
        Ch1FallingEdge,
        Ch2RisingEdge,
        Ch2FallingEdge
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

    void restart(const QString &name);
    void stop();

private:
    enum Command
    {
        PollCommand,
        MeasureCommand,
        MeasureBurstCommand
    };

    enum State
    {
        IdleState,
        ReadyState,
        TriggerState,
        Busy1State,
        Busy2State,
        Busy3State,
        Calibration1State,
        Calibration2State
    };

    struct Request
    {
        Request();

        QByteArray serialize() const;

        Command command;
        double threshold1;
        double threshold2;
        Coupling coupling1;
        Coupling coupling2;
        quint16 duration;
        Edge counterEdge;
        Clock timerClock;
        Edge startEdge;
        Edge stopEdge;

        qint8 tToV(double val) const;
    };

    struct Response
    {
        Response();

        bool deserialize(const QByteArray &data);

        quint16 state;
        quint16 voltage;
        quint32 counter;
        quint32 timer;
        quint8 startDivident;
        quint8 startDivider;
        quint8 stopDivident;
        quint8 stopDivider;

        double toTime(double clock) const;
        double toFrequency(double clock) const;
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
};

#endif // DEVICE_H
