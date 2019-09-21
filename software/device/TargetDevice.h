#ifndef TARGETDEVICE_H
#define TARGETDEVICE_H

#include <QTime>
#include "Device.h"
#include "DeviceController.h"

class QTimer;
class QSerialPort;

class TargetDevice : public Device
{
    Q_OBJECT

public:
    explicit TargetDevice(QObject *parent = 0);

    void reconnect();
    bool isConnected() const;

    void restart();
    bool isStarted() const;

private:
    void process(const QByteArray &data);

    void read();
    void write(const QByteArray &data);

    void get(quint8 data);

    void put32(quint32 data);
    void put(quint8 data);
    void flush();

    quint8 checksum(const QByteArray &data);

private:
    QTimer * const m_timer;
    QSerialPort * const m_port;

    bool m_req;

    QByteArray m_inbox;
    bool m_escape;
    QByteArray m_outbox;
};

#endif // TARGETDEVICE_H
