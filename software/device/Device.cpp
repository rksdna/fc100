#include <QDebug>
#include <QTimer>
#include <QSettings>
#include <QDataStream>
#include <QSerialPort>
#include <QSerialPortInfo>
#include "Device.h"
#include "DeviceChannel.h"
#include "DeviceReference.h"
#include "DeviceProcessor.h"
#include "DeviceController.h"

Device::Device(QObject *parent)
    : QObject(parent),
      m_timer(new QTimer(this)),
      m_port(new QSerialPort(this)),
      m_reference(new DeviceReference(this)),
      m_channel1(new DeviceChannel(this)),
      m_channel2(new DeviceChannel(this)),
      m_controller(new DeviceController(this)),
      m_processor(new DeviceProcessor(this)),
      m_ready(false),
      m_escape(false),
      m_state(IdleState)
{
    m_timer->setSingleShot(true);
    connect(m_timer, &QTimer::timeout, this, &Device::open);

    connect(m_port, &QSerialPort::readyRead, this, &Device::read);

    connect(m_controller, &DeviceController::controlInvalidated, this, &Device::clear);

    connect(m_processor, &DeviceProcessor::cacheInvalidated, this, &Device::clear);
    connect(m_processor, &DeviceProcessor::cacheFilled, this, &Device::stop);
}

DeviceReference *Device::reference() const
{
    return m_reference;
}

DeviceChannel *Device::channel1() const
{
    return m_channel1;
}

DeviceChannel *Device::channel2() const
{
    return m_channel2;
}

DeviceController *Device::controller() const
{
    return m_controller;
}

DeviceProcessor *Device::processor() const
{
    return m_processor;
}

QString Device::portName() const
{
    return m_portName;
}

void Device::setPortName(const QString &name)
{
    if (m_portName != name)
    {
        m_portName = name;

        open();
    }
}

void Device::open()
{
    close();

    m_port->setPortName(autoPortName());
    if (m_port->open(QSerialPort::ReadWrite))
    {
        m_escape = false;
        process(QByteArray());
    }
    else
    {
        m_timer->start(500);
    }
}

void Device::close()
{
    setReady(false);
    if (m_port->isOpen())
        m_port->close();
}

bool Device::isReady() const
{
    return m_ready;
}

void Device::clear()
{
    m_processor->clear();

    if (m_state != IdleState)
        m_state = TriggerState;
}

void Device::start()
{
    m_state = TriggerState;
}

void Device::stop()
{
    m_controller->setTrigger(DeviceController::ManualTrigger);
}

void Device::saveToSettings(QSettings &settings) const
{
    settings.setValue("PortName", portName());

    settings.beginGroup("Reference");
    m_reference->saveToSettings(settings);
    settings.endGroup();

    settings.beginGroup("Channel1");
    m_channel1->saveToSettings(settings);
    settings.endGroup();

    settings.beginGroup("Channel2");
    m_channel2->saveToSettings(settings);
    settings.endGroup();

    settings.beginGroup("Controller");
    m_controller->saveToSettings(settings);
    settings.endGroup();

    settings.beginGroup("Processor");
    m_processor->saveToSettings(settings);
    settings.endGroup();
}

void Device::restoreFromSettings(QSettings &settings)
{
    setPortName(settings.value("PortName").toString());

    settings.beginGroup("Reference");
    m_reference->restoreFromSettings(settings);
    settings.endGroup();

    settings.beginGroup("Channel1");
    m_channel1->restoreFromSettings(settings);
    settings.endGroup();

    settings.beginGroup("Channel2");
    m_channel2->restoreFromSettings(settings);
    settings.endGroup();

    settings.beginGroup("Controller");
    m_controller->restoreFromSettings(settings);
    settings.endGroup();

    settings.beginGroup("Processor");
    m_processor->restoreFromSettings(settings);
    settings.endGroup();
}

QString Device::autoPortName() const
{
    if (m_portName.isEmpty())
    {
        foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
        {
            if (info.vendorIdentifier() == 0x0483 && info.productIdentifier() == 0x5740 && !info.isBusy())
                return info.systemLocation();
        }
    }

    return m_portName;
}

void Device::setReady(bool state)
{
    if (m_ready != state)
    {
        m_ready = state;
        emit readyChanged(m_ready);
    }
}

void Device::process(const QByteArray &replyData)
{
    quint32 status;
    quint32 counter1;
    quint32 timer1;
    quint32 counter2;
    quint32 timer2;
    quint8 start1;
    quint8 stop1;
    quint8 start2;
    quint8 stop2;
    quint8 startFullScale;
    quint8 stopFullScale;
    quint8 startZeroScale;
    quint8 stopZeroScale;

    QDataStream reply(replyData);
    reply.setByteOrder(QDataStream::LittleEndian);
    reply >> status;
    reply >> counter1;
    reply >> timer1;
    reply >> counter2;
    reply >> timer2;
    reply >> start1;
    reply >> stop1;
    reply >> start2;
    reply >> stop2;
    reply >> startFullScale;
    reply >> stopFullScale;
    reply >> startZeroScale;
    reply >> stopZeroScale;

    setReady(reply.atEnd() && reply.status() == QDataStream::Ok);
    if (isReady())
    {
        if (m_state == BusyState && status == DoneStatus)
        {
            const qreal clock = reference()->frequency();

            const qreal start = startFullScale - startZeroScale;
            const qreal stop = stopFullScale - stopZeroScale;

            const qreal qStart1 = qreal(start1) / start;
            const qreal qStop1 = qreal(stop1) / stop;
            const qreal qStart2 = qreal(start2) / start;
            const qreal qStop2 = qreal(stop2) / stop;

            const qreal qTimer1 = qreal(timer1) + qStart1 - qStop1;
            const qreal qCounter1 = counter1;
            const qreal qTimer2 = qreal(timer2) + qStart2 - qStop2;

            switch (controller()->mode())
            {
            case DeviceController::TimeMode:
            case DeviceController::PeriodMode:
            case DeviceController::GatePeriodMode:
                m_processor->take(DeviceProcessor::TimeType, qTimer1 / qCounter1 / clock, qTimer1 / clock);
                break;

            case DeviceController::DutyMode:
                m_processor->take(DeviceProcessor::DutyType, 100.0 * qTimer1 / qTimer2, qTimer2 / clock);
                break;

            case DeviceController::FrequencyMode:
            case DeviceController::GateFrequencyMode:
                m_processor->take(DeviceProcessor::FrequencyType, qCounter1 / qTimer1 * clock, qTimer1 / clock);
                break;

            case DeviceController::CountMode:
            case DeviceController::GateCountMode:
                m_processor->take(DeviceProcessor::NoType, qCounter1, qTimer1 / clock);
                break;
            }

            m_state = controller()->trigger() == DeviceController::AutoTrigger ? TriggerState : IdleState;
        }

        if (m_state == BusyState && status == FailStatus)
        {
            switch (controller()->mode())
            {
            case DeviceController::TimeMode:
            case DeviceController::PeriodMode:
            case DeviceController::GatePeriodMode:
                m_processor->take(DeviceProcessor::TimeType, qQNaN(), qQNaN());
                break;

            case DeviceController::DutyMode:
                m_processor->take(DeviceProcessor::DutyType, qQNaN(), qQNaN());
                break;

            case DeviceController::FrequencyMode:
            case DeviceController::GateFrequencyMode:
                m_processor->take(DeviceProcessor::FrequencyType, qQNaN(), qQNaN());
                break;

            case DeviceController::CountMode:
            case DeviceController::GateCountMode:
                m_processor->take(DeviceProcessor::NoType, qQNaN(), qQNaN());
                break;
            }

            m_state = controller()->trigger() == DeviceController::AutoTrigger ? TriggerState : IdleState;
        }
    }

    QByteArray requestData;
    QDataStream requestStream(&requestData, QIODevice::WriteOnly);
    requestStream.setByteOrder(QDataStream::LittleEndian);
    if (m_state == TriggerState)
    {
        switch (controller()->mode())
        {
        case DeviceController::TimeMode:
            requestStream << quint32(MeasureTimeCommand);
            break;

        case DeviceController::FrequencyMode:
        case DeviceController::PeriodMode:
        case DeviceController::CountMode:
            requestStream << quint32(MeasureFrequencyCommand);
            break;

        case DeviceController::DutyMode:
            requestStream << quint32(MeasureDutyCommand);
            break;

        case DeviceController::GateFrequencyMode:
        case DeviceController::GatePeriodMode:
        case DeviceController::GateCountMode:
            requestStream << quint32(MeasureGateFrequencyCommand);
            break;

        default:
            break;
        }

        m_state = BusyState;
    }
    else
    {
        requestStream << quint32(PollCommand);
    }

    requestStream << quint32(m_controller->duration());
    requestStream << quint8(m_channel1->threshold());
    requestStream << quint8(m_channel2->threshold());
    requestStream << quint8(m_channel1->coupling());
    requestStream << quint8(m_channel2->coupling());
    requestStream << quint8(m_reference->source());
    requestStream << quint8(m_controller->startEvent());
    requestStream << quint8(m_controller->stopEvent());
    requestStream << quint8(m_controller->countEvent());

    write(requestData);
}

void Device::read()
{
    foreach (char ch, m_port->readAll())
        get(ch);
}

void Device::write(const QByteArray &data)
{
    foreach (char ch, data)
        put(ch);

    put(checksum(data));
    m_outbox.append(0xC0);
    flush();
}

void Device::get(quint8 data)
{
    switch (data)
    {
    case 0xC0:
        if (m_inbox.size() > 1 && checksum(m_inbox) == 0)
        {
            m_inbox.chop(1);
            process(m_inbox);
        }

        m_inbox.clear();
        m_escape = false;
        break;

    case 0xDB:
        m_escape = true;
        break;

    case 0xDC:
        m_inbox.append(m_escape ? 0xC0 : 0xDC);
        m_escape = false;
        break;

    case 0xDD:
        m_inbox.append(m_escape ? 0xDB : 0xDD);
        m_escape = false;
        break;

    default:
        m_inbox.append(data);
        m_escape = false;
        break;
    }
}

void Device::put(quint8 data)
{
    switch (data)
    {
    case 0xC0:
        m_outbox.append(0xDB);
        m_outbox.append(0xDC);
        break;

    case 0xDB:
        m_outbox.append(0xDB);
        m_outbox.append(0xDD);
        break;

    default:
        m_outbox.append(data);
        break;
    }
}

void Device::flush()
{
    m_port->write(m_outbox);
    m_outbox.clear();
    m_timer->start(250);
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
