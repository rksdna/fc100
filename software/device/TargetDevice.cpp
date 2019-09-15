#include <QDebug>
#include <QTimer>
#include <QSerialPort>
#include <QSerialPortInfo>
#include "Regs.h"
#include "TargetDevice.h"
#include "DeviceChannel.h"
#include "DeviceReference.h"
#include "DeviceController.h"
#include "DeviceProcessor.h"
#include "DeviceChannel.h"
#include "DeviceReference.h"

static quint8 dac(int threshold)
{
    return 255 - threshold;
}

static quint8 hpf1(DeviceChannel::Coupling coupling)
{
    switch (coupling)
    {
    case DeviceChannel::DcCoupling:
        return 0;

    case DeviceChannel::AcCoupling:
        return COUNTER_CTRL_HPF_CH1;

    default:
        break;
    }

    return 0;
}

static quint8 hpf2(DeviceChannel::Coupling coupling)
{
    switch (coupling)
    {
    case DeviceChannel::DcCoupling:
        return 0;

    case DeviceChannel::AcCoupling:
        return COUNTER_CTRL_HPF_CH2;

    default:
        break;
    }

    return 0;
}

static quint8 start1(DeviceController::Event event)
{
    switch (event)
    {
    case DeviceController::Ch1RisingEdgeEvent:
        return COUNTER_MODE_STRT_CH1R;

    case DeviceController::Ch1FallingEdgeEvent:
        return COUNTER_MODE_STRT_CH1F;

    case DeviceController::Ch2RisingEdgeEvent:
        return COUNTER_MODE_STRT_CH2R;

    case DeviceController::Ch2FallingEdgeEvent:
        return COUNTER_MODE_STRT_CH2F;

    default:
        break;
    }

    return 0;
}

static quint8 stop1(DeviceController::Event event)
{
    switch (event)
    {
    case DeviceController::Ch1RisingEdgeEvent:
        return COUNTER_MODE_STOP_CH1R;

    case DeviceController::Ch1FallingEdgeEvent:
        return COUNTER_MODE_STOP_CH1F;

    case DeviceController::Ch2RisingEdgeEvent:
        return COUNTER_MODE_STOP_CH2R;

    case DeviceController::Ch2FallingEdgeEvent:
        return COUNTER_MODE_STOP_CH2F;

    default:
        break;
    }

    return 0;
}

static quint8 count(DeviceController::Event event)
{
    switch (event)
    {
    case DeviceController::Ch1RisingEdgeEvent:
        return COUNTER_MODE_CNT_CH1R;

    case DeviceController::Ch1FallingEdgeEvent:
        return COUNTER_MODE_CNT_CH1F;

    case DeviceController::Ch2RisingEdgeEvent:
        return COUNTER_MODE_CNT_CH2R;

    case DeviceController::Ch2FallingEdgeEvent:
        return COUNTER_MODE_CNT_CH2F;

    default:
        break;
    }

    return 0;
}

static quint8 ref(DeviceReference::Source source)
{
    switch (source)
    {
    case DeviceReference::InternalSource:
        return COUNTER_MODE_TMR_CLK;

    case DeviceReference::ExternalSource:
        return COUNTER_MODE_TMR_REF;

    default:
        break;
    }

    return 0;
}

TargetDevice::TargetDevice(QObject *parent)
    : Device(parent),
      m_timer(new QTimer(this)),
      m_port(new QSerialPort(this)),
      m_state(IdleState),
      m_measure(false)
{
    m_timer->setSingleShot(true);
    connect(m_timer, &QTimer::timeout, this, &TargetDevice::open);
    connect(m_port, &QSerialPort::readyRead, this, &TargetDevice::read);
}

void TargetDevice::open()
{
    setReady(false);

    if (m_port->isOpen())
        m_port->close();

    m_port->setPortName(preparePortName(portName()));
    if (m_port->open(QSerialPort::ReadWrite))
    {
        write(m_regs.serialize());
    }
    else
        m_timer->start(500);
}

void TargetDevice::measure()
{
    m_state = IdleState;
    m_measure = true;
}

QString TargetDevice::preparePortName(const QString &name) const
{
    if (!name.isEmpty())
        return name;

    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        if (info.vendorIdentifier() == 0x0483 && info.productIdentifier() == 0x5740 && !info.isBusy())
            return info.systemLocation();
    }

    return QString();
}

TargetDevice::State TargetDevice::fail() const
{
    processor()->take(m_type, qQNaN(), qQNaN());
    return IdleState;
}

TargetDevice::State TargetDevice::done() const
{
    const qreal clock = reference()->frequency();
    const qreal counter = m_regs.cnt;
    const qreal start = qreal(m_regs.tac_strt) / m_tac_start;
    const qreal stop = qreal(m_regs.tac_stop) / m_tac_stop;
    const qreal timer = qreal(m_regs.tmr) + start - stop;

    switch (m_type)
    {
    case DeviceProcessor::NoType:
        processor()->take(m_type, counter, timer / clock);
        break;

    case DeviceProcessor::DutyType:

        processor()->take(m_type, m_ti / timer, timer / clock);
        break;

    case DeviceProcessor::TimeType:
        processor()->take(m_type, timer / counter / clock, timer / clock);
        break;

    case DeviceProcessor::FrequencyType:
        processor()->take(m_type, counter / timer * clock, timer / clock);
        break;

    default:
        processor()->take(m_type, qQNaN(), timer / clock);
        break;
    }

    return IdleState;
}

TargetDevice::State TargetDevice::proc(State state, int elapsed)
{
    setReady(m_regs.id == 0xAA);

    const bool is_started = m_regs.ack & COUNTER_ACK_STRT;
    const bool is_stopped = m_regs.ack & COUNTER_ACK_STOP;
    const bool is_ready = is_started && is_stopped;

    switch (state)
    {
    case IdleState:
        m_regs.dac1 = dac(channel1()->threshold());
        m_regs.dac2 = dac(channel2()->threshold());
        m_regs.mode = ref(reference()->source());
        m_regs.ctrl =
                hpf1(channel1()->coupling()) |
                hpf2(channel2()->coupling()) |
                COUNTER_CTRL_CLR;

        if (m_measure)
        {
            m_measure = controller()->trigger() == DeviceController::AutoTrigger;
            return CalibrateFullScaleState;
        }

        break;

    case CalibrateFullScaleState:
        if (is_ready)
        {
            m_regs.ctrl |= COUNTER_CTRL_CLR;
            m_regs.ctrl &= ~COUNTER_CTRL_CLB_FS;

            m_tac_start = m_regs.tac_strt;
            m_tac_stop = m_regs.tac_stop;
            return CalibrateZeroScaleState;
        }

        m_regs.ctrl &= ~COUNTER_CTRL_CLR;
        m_regs.ctrl |= COUNTER_CTRL_CLB_FS;
        break;

    case CalibrateZeroScaleState:
        if (is_ready)
        {
            m_regs.ctrl |= COUNTER_CTRL_CLR;
            m_regs.ctrl &= ~COUNTER_CTRL_CLB_ZS;

            m_tac_start -= m_regs.tac_strt;
            m_tac_stop -= m_regs.tac_stop;

            switch (controller()->mode())
            {
            case DeviceController::TimeMode:
                m_type = DeviceProcessor::TimeType;
                return Single1State;

            case DeviceController::FrequencyMode:
                m_type = DeviceProcessor::FrequencyType;
                return Burst1State;

            case DeviceController::PeriodMode:
                m_type = DeviceProcessor::TimeType;
                return Burst1State;

            case DeviceController::CountMode:
                m_type = DeviceProcessor::NoType;
                return Burst1State;

            case DeviceController::DutyMode:
                m_type = DeviceProcessor::DutyType;
                return Ps1State;

            case DeviceController::GateFrequencyMode:
                m_type = DeviceProcessor::FrequencyType;
                return Burst0State;

            case DeviceController::GatePeriodMode:
                m_type = DeviceProcessor::TimeType;
                return Burst0State;

            case DeviceController::GateCountMode:
                m_type = DeviceProcessor::NoType;
                return Burst0State;

            default:
                break;
            }

            return IdleState;
        }

        m_regs.ctrl &= ~COUNTER_CTRL_CLR;
        m_regs.ctrl |= COUNTER_CTRL_CLB_ZS;
        break;

    case Ps1State:
        m_duration = controller()->duration();
        m_regs.mode |= start1(controller()->countEvent());
        m_regs.mode |= stop1(controller()->complementCountEvent());
        m_regs.mode |= count(controller()->complementCountEvent());
        m_regs.ctrl &= ~COUNTER_CTRL_CLR;
        m_regs.ctrl |= COUNTER_CTRL_STRT | COUNTER_CTRL_STOP;
        return Ps2State;

    case Ps2State:
        if (is_started)
        {
            m_regs.ctrl |= COUNTER_CTRL_TEST;
            return Ps3State;
        }

        if (elapsed > m_duration)
            return fail();

        break;

    case Ps3State:
        if (is_ready)
        {
            const qreal start = qreal(m_regs.tac_strt) / m_tac_start;
            const qreal stop = qreal(m_regs.tac_stop) / m_tac_stop;
            const qreal timer = qreal(m_regs.tmr) + start - stop;

            m_ti = timer;

            m_regs.mode ^= COUNTER_MODE_STOP_0;
            m_regs.mode ^= COUNTER_MODE_CNT_0;
            m_regs.ctrl |= COUNTER_CTRL_CLR;
            m_regs.ctrl &= ~COUNTER_CTRL_TEST;
            m_regs.ctrl &= ~COUNTER_CTRL_STOP;
            m_regs.ctrl &= ~COUNTER_CTRL_STRT;

            return Ps4State;
        }

        if (elapsed > m_duration)
            return fail();

        break;

    case Ps4State:
        m_regs.ctrl &= ~COUNTER_CTRL_CLR;
        m_regs.ctrl |= COUNTER_CTRL_STRT | COUNTER_CTRL_STOP;
        return Burst2State;

    case Single1State:
        m_duration = controller()->duration();
        m_regs.mode |= start1(controller()->startEvent());
        m_regs.mode |= stop1(controller()->stopEvent());
        m_regs.mode |= count(controller()->stopEvent());
        m_regs.ctrl &= ~COUNTER_CTRL_CLR;
        m_regs.ctrl |= COUNTER_CTRL_STRT | COUNTER_CTRL_STOP;
        return Burst2State;

    case Burst0State:
        m_duration = controller()->duration();
        m_regs.mode |= start1(controller()->startEvent());
        m_regs.mode |= stop1(controller()->stopEvent());
        m_regs.mode |= count(controller()->countEvent());
        m_regs.ctrl &= ~COUNTER_CTRL_CLR;
        m_regs.ctrl |= COUNTER_CTRL_STRT | COUNTER_CTRL_STOP;
        return Burst2State;

    case Burst1State:
        m_duration = controller()->duration();
        m_regs.mode |= start1(controller()->countEvent());
        m_regs.mode |= stop1(controller()->countEvent());
        m_regs.mode |= count(controller()->countEvent());
        m_regs.ctrl &= ~COUNTER_CTRL_CLR;
        m_regs.ctrl |= COUNTER_CTRL_STRT;
        return Burst2State;

    case Burst2State:
        if (is_started)
        {
            m_regs.ctrl |= COUNTER_CTRL_TEST;
            return Burst3State;
        }

        if (elapsed > m_duration)
            return fail();

        break;

    case Burst3State:
        if (elapsed > m_duration)
        {
            m_regs.ctrl |= COUNTER_CTRL_STOP;
            return Burst4State;
        }

        break;

    case Burst4State:
        if (is_ready)
        {
            return done();
        }

        if (elapsed > m_duration)
            return fail();

        break;

    default:
        break;
    }

    return state;
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
                const State state = proc(m_state, m_time.elapsed());
                if (state != m_state)
                {
                    m_state = state;
                    m_time.restart();
                }

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

