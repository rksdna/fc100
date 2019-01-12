#include <QDebug>
#include <QTimer>
#include <QDateTime>
#include <QDataStream>
#include <QSerialPort>
#include "Device.h"

/*
 * MODE START STOP COUNTER DURATION DISPLAY
 * F    A/B   A/B  A/B     1mS-10S  COUNTER/TIMER, Hz
 * T    A/B   A/B  A/B     1mS-10S  TIMER/COUNTER, S
 * RPM  A/B   A/B  A/B     1mS-10S  COUNTER/TIMER * 60, RPM
 * CNT  A/B   A/B  A/B     inf      COUNTER, 1
 * ARM  B/A  !B/A  A/B     0        COUNTER/TIMER
 * tP   A/B   A/B  A/B     0        TIMER, S
 * tL  !A/B   A/B  A/B     0        TIMER, S
 * tH   A/B  !A/B  A/B     0        TIMER, S
 * tD   A/B   B/A  A/B     0        TIMER, S
 */

Device::Request::Request()
    : threshold1(0),
      threshold2(0),
      coupling1(0),
      coupling2(0),
      command(0),
      duration(100),
      counterEdge(0),
      timerClock(0),
      startEdge(0),
      stopEdge(0)
{
}

QByteArray Device::Request::serialize() const
{
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    stream.setByteOrder(QDataStream::LittleEndian);

    stream << threshold1;
    stream << threshold2;
    stream << coupling1;
    stream << coupling2;

   /* if (command != PollCommand)
    {
        stream << quint16(command == MeasureBurstCommand);
        stream << quint16(duration);
        stream << quint8(counterEdge);
        stream << quint8(timerClock);
        stream << quint8(startEdge);
        stream << quint8(stopEdge);
    }*/

    return data;
}

Device::Response::Response()
    : state(0),
      voltage(0),
      counter(0),
      timer(0),
      startDivident(0),
      startDivider(0),
      stopDivident(0),
      stopDivider(0)
{
}

bool Device::Response::deserialize(const QByteArray &data)
{
    QDataStream stream(data);
    stream.setByteOrder(QDataStream::LittleEndian);

    stream >> state;
    stream >> voltage;

    if (state == ReadyState)
    {
        stream >> counter;
        stream >> timer;
        stream >> startDivident;
        stream >> startDivider;
        stream >> stopDivident;
        stream >> stopDivider;

        //qDebug() << startDivident << startDivider << stopDivident << stopDivider << counter << timer;
    }

    return stream.status() == QDataStream::Ok && stream.atEnd();
}

double Device::Response::toTime(double clock) const
{
    const double events = counter;
    const double start = double(startDivident) / startDivider;
    const double stop = double(stopDivident) / stopDivider;
    const double time = double(timer) + start - stop;
    return time / clock / events;
}

double Device::Response::toFrequency(double clock) const
{
    const double events = counter;
    const double start = double(startDivident) / startDivider;
    const double stop = double(stopDivident) / stopDivider;
    const double time = double(timer) + start - stop;
    return clock * events / time;
}

quint8 Device::checksum(const QByteArray &data)
{
    // CRC-8
    // Poly 0x07
    // Init 0x00
    // RefIn false
    // RefOut false
    // Xor 0x00

    static const quint8 table[] =
    {
        0x00, 0x07, 0x0E, 0x09, 0x1C, 0x1B, 0x12, 0x15,
        0x38, 0x3F, 0x36, 0x31, 0x24, 0x23, 0x2A, 0x2D,
        0x70, 0x77, 0x7E, 0x79, 0x6C, 0x6B, 0x62, 0x65,
        0x48, 0x4F, 0x46, 0x41, 0x54, 0x53, 0x5A, 0x5D,
        0xE0, 0xE7, 0xEE, 0xE9, 0xFC, 0xFB, 0xF2, 0xF5,
        0xD8, 0xDF, 0xD6, 0xD1, 0xC4, 0xC3, 0xCA, 0xCD,
        0x90, 0x97, 0x9E, 0x99, 0x8C, 0x8B, 0x82, 0x85,
        0xA8, 0xAF, 0xA6, 0xA1, 0xB4, 0xB3, 0xBA, 0xBD,
        0xC7, 0xC0, 0xC9, 0xCE, 0xDB, 0xDC, 0xD5, 0xD2,
        0xFF, 0xF8, 0xF1, 0xF6, 0xE3, 0xE4, 0xED, 0xEA,
        0xB7, 0xB0, 0xB9, 0xBE, 0xAB, 0xAC, 0xA5, 0xA2,
        0x8F, 0x88, 0x81, 0x86, 0x93, 0x94, 0x9D, 0x9A,
        0x27, 0x20, 0x29, 0x2E, 0x3B, 0x3C, 0x35, 0x32,
        0x1F, 0x18, 0x11, 0x16, 0x03, 0x04, 0x0D, 0x0A,
        0x57, 0x50, 0x59, 0x5E, 0x4B, 0x4C, 0x45, 0x42,
        0x6F, 0x68, 0x61, 0x66, 0x73, 0x74, 0x7D, 0x7A,
        0x89, 0x8E, 0x87, 0x80, 0x95, 0x92, 0x9B, 0x9C,
        0xB1, 0xB6, 0xBF, 0xB8, 0xAD, 0xAA, 0xA3, 0xA4,
        0xF9, 0xFE, 0xF7, 0xF0, 0xE5, 0xE2, 0xEB, 0xEC,
        0xC1, 0xC6, 0xCF, 0xC8, 0xDD, 0xDA, 0xD3, 0xD4,
        0x69, 0x6E, 0x67, 0x60, 0x75, 0x72, 0x7B, 0x7C,
        0x51, 0x56, 0x5F, 0x58, 0x4D, 0x4A, 0x43, 0x44,
        0x19, 0x1E, 0x17, 0x10, 0x05, 0x02, 0x0B, 0x0C,
        0x21, 0x26, 0x2F, 0x28, 0x3D, 0x3A, 0x33, 0x34,
        0x4E, 0x49, 0x40, 0x47, 0x52, 0x55, 0x5C, 0x5B,
        0x76, 0x71, 0x78, 0x7F, 0x6A, 0x6D, 0x64, 0x63,
        0x3E, 0x39, 0x30, 0x37, 0x22, 0x25, 0x2C, 0x2B,
        0x06, 0x01, 0x08, 0x0F, 0x1A, 0x1D, 0x14, 0x13,
        0xAE, 0xA9, 0xA0, 0xA7, 0xB2, 0xB5, 0xBC, 0xBB,
        0x96, 0x91, 0x98, 0x9F, 0x8A, 0x8D, 0x84, 0x83,
        0xDE, 0xD9, 0xD0, 0xD7, 0xC2, 0xC5, 0xCC, 0xCB,
        0xE6, 0xE1, 0xE8, 0xEF, 0xFA, 0xFD, 0xF4, 0xF3
    };

    quint8 crc = 0x00;
    foreach (char ch, data)
        crc = table[crc ^ quint8(ch)];

    return crc;
}

qreal Device::dac8(int code)
{
    return 5.0 * code / 128;
}

quint16 Device::vendorIdentifier()
{
    return 0x0483;
}

quint16 Device::productIdentifier()
{
    return 0x5740;
}

Device::Device(QObject *parent)
    : QObject(parent),
      m_channel1(-128, 127, dac8(-128), dac8(127)),
      m_channel2(-128, 127, dac8(-128), dac8(127)),
      m_port(new QSerialPort(this)),
      m_timer(new QTimer(this))
{
    connect(m_port, &QSerialPort::readyRead, this, &Device::onReadyRead);
    connect(m_timer, &QTimer::timeout, this, &Device::onTimeout);
}

Device::~Device()
{
}

DeviceChannel *Device::channel1()
{
    return &m_channel1;
}

DeviceChannel *Device::channel2()
{
    return &m_channel2;
}

void Device::restart(const QString &name)
{
    stop();

    m_port->setPortName(name);
    if (m_port->open(QSerialPort::ReadWrite))
        read(QByteArray());
}

void Device::stop()
{
    qDebug() << "stop";
    m_timer->stop();
    if (m_port->isOpen())
        m_port->close();
}

void Device::onReadyRead()
{
    while (m_port->canReadLine())
    {
        const QByteArray line = m_port->readLine();
        if (line.startsWith(':') && line.endsWith("\r\n"))
        {
            QByteArray data = QByteArray::fromHex(line);
            if (data.size() > 1 && checksum(data) == 0)
            {
                data.chop(1);
                read(data);
            }
        }
    }
}

void Device::onTimeout()
{
    stop();
}

void Device::read(const QByteArray &data)
{

    bool pcs = false;

    Response response;
    if (response.deserialize(data))
    {
        const double clock = 10E+6;

        if (response.state == ReadyState)
        {
            static double prev = 0;
            static double prev1 = 0;

            double value = response.toFrequency(clock);
            double error = value - 1000.655;

            prev = 0.95 * prev + 0.05 * error;
            prev1 = 0.95 * prev1 + 0.05 * value;

            qDebug().noquote() << "F =" << QString::number(value, 'f', 3) << QString::number(error, 'f', 6);
            pcs = true;
        }

        else if (response.state == IdleState)
        {
            qDebug().noquote() << "---";
            pcs = true;
        }

        m_timer->start(100);
    }

    Request request;
    request.command = pcs ? MeasureBurstCommand : PollCommand;
    request.threshold1 = qint8(m_channel1.threshold());
    request.threshold2 = qint8(m_channel2.threshold());
    request.coupling1 = quint8(m_channel1.coupling());
    request.coupling2 = quint8(m_channel2.coupling());

    qDebug() << request.threshold1;

    request.startEdge = Ch2RisingEdge;
    request.stopEdge = Ch2RisingEdge;
    request.counterEdge = Ch2RisingEdge;

    request.timerClock = InternalClock;

    request.duration = 10;

    write(request.serialize());
}

void Device::write(const QByteArray &data)
{
    QByteArray frame;
    frame.append(data);
    frame.append(checksum(data));

    m_port->write(":" + frame.toHex() + "\r\n");
}
