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

    void restart(const QString &name);
    void stop();

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
