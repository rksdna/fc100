#ifndef DEVICE_H
#define DEVICE_H

#include <QString>
#include <QObject>

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
    explicit Device(QObject *parent = 0);

private:
    enum State
    {
        IdleState,
        EvenDigitState,
        OddDigitState
    };


private:
    void onReadyRead();
    void process_message(quint8 value);
    void begin_message();
    void complete_message();
    void drop_message();


    void read(const QByteArray &data);
    void write(const QByteArray &data);

private:
    static quint8 checksum(const QByteArray &data);

private:
    QSerialPort * const m_port;
    State m_state;
    quint8 m_byte;
    QByteArray m_buffer;






};

#endif // DEVICE_H
