#include <QTimer>
#include <QDebug>
#include <QDataStream>
#include <QSerialPort>
#include "TargetDevice.h"
#include "DeviceChannel.h"
#include "DeviceReference.h"
#include "DeviceController.h"
#include "DeviceProcessor.h"
#include "DeviceChannel.h"
#include "DeviceReference.h"

#define COUNTER_DAC1 ((quint8)0x00)
#define COUNTER_DAC2 ((quint8)0x01)
#define COUNTER_MODE ((quint8)0x02)
#define COUNTER_CTRL ((quint8)0x03)
#define COUNTER_ID ((quint8)0x04)
#define COUNTER_ACK ((quint8)0x05)
#define COUNTER_TAC_START ((quint8)0x06)
#define COUNTER_TAC_STOP ((quint8)0x07)
#define COUNTER_CNT ((quint8)0x08)
#define COUNTER_TMR ((quint8)0x0C)

#define COUNTER_DAC_DAC ((quint8)0xFF)

#define COUNTER_MODE_STRT ((quint8)0x03)
#define COUNTER_MODE_STRT_0 ((quint8)0x01)
#define COUNTER_MODE_STRT_1 ((quint8)0x02)
#define COUNTER_MODE_STRT_CH1R ((quint8)0x00)
#define COUNTER_MODE_STRT_CH1F ((quint8)0x01)
#define COUNTER_MODE_STRT_CH2R ((quint8)0x02)
#define COUNTER_MODE_STRT_CH2F ((quint8)0x03)

#define COUNTER_MODE_STOP ((quint8)0x0C)
#define COUNTER_MODE_STOP_0 ((quint8)0x04)
#define COUNTER_MODE_STOP_1 ((quint8)0x08)
#define COUNTER_MODE_STOP_CH1R ((quint8)0x00)
#define COUNTER_MODE_STOP_CH1F ((quint8)0x04)
#define COUNTER_MODE_STOP_CH2R ((quint8)0x08)
#define COUNTER_MODE_STOP_CH2F ((quint8)0x0C)

#define COUNTER_MODE_CNT ((quint8)0x30)
#define COUNTER_MODE_CNT_0 ((quint8)0x10)
#define COUNTER_MODE_CNT_1 ((quint8)0x20)
#define COUNTER_MODE_CNT_CH1R ((quint8)0x00)
#define COUNTER_MODE_CNT_CH1F ((quint8)0x10)
#define COUNTER_MODE_CNT_CH2R ((quint8)0x20)
#define COUNTER_MODE_CNT_CH2F ((quint8)0x30)

#define COUNTER_MODE_TMR ((quint8)0x40)
#define COUNTER_MODE_TMR_CLK ((quint8)0x00)
#define COUNTER_MODE_TMR_REF ((quint8)0x40)

#define COUNTER_MODE_CLR ((quint8)0x80)

#define COUNTER_CTRL_STRT ((quint8)0x01)
#define COUNTER_CTRL_STOP ((quint8)0x02)
#define COUNTER_CTRL_CLB_ZS ((quint8)0x04)
#define COUNTER_CTRL_CLB_FS ((quint8)0x08)
#define COUNTER_CTRL_HPF_CH1 ((quint8)0x10)
#define COUNTER_CTRL_HPF_CH2 ((quint8)0x20)
#define COUNTER_CTRL_TEST ((quint8)0x40)
#define COUNTER_CTRL_CLR ((quint8)0x80)

#define COUNTER_TAC_TAC ((quint8)0xFF)

#define COUNTER_ACK_STOP ((quint8)0x01)
#define COUNTER_ACK_STRT ((quint8)0x02)

#define COUNTER_ID_ID ((quint8)0xFF)

#define COUNTER_CNT_CNT ((quint32)0xFFFFFFFF)
#define COUNTER_TMR_TMR ((quint32)0xFFFFFFFF)

TargetDevice::Regs::Regs()
    : dac1(0),
      dac2(0),
      mode(0),
      ctrl(0),
      id(0),
      ack(0),
      tac_strt(0),
      tac_stop(0),
      cnt(0),
      tmr(0)
{
}

QByteArray TargetDevice::Regs::serialize() const
{
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    stream.setByteOrder(QDataStream::LittleEndian);

    stream << dac1;
    stream << dac2;
    stream << mode;
    stream << ctrl;

    return data;
}

void TargetDevice::Regs::deserialize(const QByteArray &data)
{
    QDataStream stream(data);
    stream.setByteOrder(QDataStream::LittleEndian);

    stream >> id;
    stream >> ack;
    stream >> tac_strt;
    stream >> tac_stop;
    stream >> cnt;
    stream >> tmr;
}

TargetDevice::TargetDevice(QObject *parent)
    : Device(parent),
      m_timer(new QTimer(this)),
      m_port(new QSerialPort(this)),
      m_state(IdleState)
{
    connect(m_timer, &QTimer::timeout, this, &TargetDevice::open);
    connect(m_port, &QSerialPort::readyRead, this, &TargetDevice::read);
}

void TargetDevice::open()
{
    setReady(false);

    if (m_port->isOpen())
        m_port->close();

    m_port->setPortName(portName());
    if (m_port->open(QSerialPort::ReadWrite))
    {
        write(m_regs.serialize());
    }
    else
        m_timer->start(500);
}

void TargetDevice::measure()
{
    m_tm.restart();
    m_state = T1State;
}

TargetDevice::State TargetDevice::proc(State state)
{
    setReady(m_regs.id == 0xAA);

    const bool is_started = m_regs.ack & COUNTER_ACK_STRT;
    const bool is_stopped = m_regs.ack & COUNTER_ACK_STOP;
    const bool is_ready = is_started && is_stopped;
    const bool is_free = !is_started && !is_stopped;

    switch (state)
    {
    case IdleState:
        m_regs.dac1 = 255 - channel1()->threshold();
        m_regs.dac2 = 255 - channel2()->threshold();
        m_regs.mode = COUNTER_MODE_CLR;
        m_regs.ctrl = COUNTER_CTRL_CLR;
        break;

    case T1State:
        m_regs.mode = COUNTER_MODE_CLR;
        m_regs.ctrl = COUNTER_CTRL_CLR;
        return is_free ? T1TState : T1State;

    case T1TState:
        if (is_ready)
        {
            qDebug() << "+" << m_regs.tac_strt;
            qDebug() << "+" << m_regs.tac_stop;
            return T2State;
        }

        m_regs.mode = 0;
        m_regs.ctrl = COUNTER_CTRL_CLB_ZS;
        return T1TState;

    case T2State:
        m_regs.mode = COUNTER_MODE_CLR;
        m_regs.ctrl = COUNTER_CTRL_CLR;
        return is_free ? T2TState : T2State;

    case T2TState:
        if (is_ready)
        {
            qDebug() << "-" << m_regs.tac_strt;
            qDebug() << "-" << m_regs.tac_stop;
            return IdleState;
        }

        m_regs.mode = 0;
        m_regs.ctrl = COUNTER_CTRL_CLB_FS;
        return T2TState;

        break;
    default:
        break;
    }

    return state;


    /*m_regs.ctrl |= COUNTER_CTRL_TEST;

    switch (channel1()->coupling())
    {
    case DeviceChannel::DcCoupling:
        m_regs.ctrl &= ~COUNTER_CTRL_HPF_CH1;
        break;

    case DeviceChannel::AcCoupling:
        m_regs.ctrl |= COUNTER_CTRL_HPF_CH1;
        break;

    default:
        break;
    }

    switch (channel2()->coupling())
    {
    case DeviceChannel::DcCoupling:
        m_regs.ctrl &= ~COUNTER_CTRL_HPF_CH2;
        break;

    case DeviceChannel::AcCoupling:
        m_regs.ctrl |= COUNTER_CTRL_HPF_CH2;
        break;

    default:
        break;
    }*/
}


void TargetDevice::write(const QByteArray &data)
{
    QByteArray frame;
    frame.append(data);
    frame.append(checksum(data));

    m_port->write(":" + frame.toHex() + "\r\n");
    m_timer->start(250);
}

void TargetDevice::read()
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
                m_regs.deserialize(data);
                m_state = proc(m_state);
                write(m_regs.serialize());
            }
        }
    }
}

quint8 TargetDevice::checksum(const QByteArray &data)
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

