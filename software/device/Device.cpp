#include <QDebug>
#include "Device.h"
#include "MockDevice.h"

Device *Device::createDevice(const QString &type, QObject *parent)
{
    Q_UNUSED(type)

    return new MockDevice(parent);
}

Device::Device(QObject *parent)
    : QObject(parent),
      m_timer(new QTimer()),
      m_ch1Coupling(DcCoupling),
      m_ch1Probe(x10Probe),
      m_ch1Threshold(128),
      m_ch2Coupling(DcCoupling),
      m_ch2Probe(x10Probe),
      m_ch2Threshold(128),
      m_clock(InternalClock),
      m_clockFrequency(10000000.0),
      m_trigger(AutoTrigger),
      m_mode(FrequencyMode),
      m_countEventEnabled(true),
      m_countEvent(Ch1RisingEdgeEvent),
      m_startStopEventEnabled(false),
      m_startEvent(Ch1RisingEdgeEvent),
      m_stopEvent(Ch1RisingEdgeEvent),
      m_duration(100),
      m_maxSamplesCount(10),
      m_measure(false),
      m_delay(false)
{
    m_timer->setSingleShot(true);
    m_timer->setTimerType(Qt::PreciseTimer);
    connect(m_timer, &QTimer::timeout, this, &Device::timeout);
}

void Device::reset()
{
    emit ch1CouplingChanged(m_ch1Coupling);
    emit ch1ProbeChanged(m_ch1Probe);
    emit ch1ThresholdChanged(m_ch1Threshold);
    emit ch1DescriptionChanged(description(m_ch1Threshold, m_ch1Probe));

    emit ch2CouplingChanged(m_ch2Coupling);
    emit ch2ProbeChanged(m_ch2Probe);
    emit ch2ThresholdChanged(m_ch2Threshold);
    emit ch2DescriptionChanged(description(m_ch2Threshold, m_ch2Probe));

    emit clockChanged(m_clock);
    emit clockFrequencyChanged(m_clockFrequency);
    emit triggerChanged(m_trigger);
    emit modeChanged(m_mode);
    emit countEventEnabled(m_countEventEnabled);
    emit startStopEventEnabled(m_startStopEventEnabled);
    emit countEventChanged(m_countEvent);
    emit startEventChanged(m_startEvent);
    emit stopEventChanged(m_stopEvent);
    emit durationChanged(m_duration);
}

void Device::restart()
{
    qDebug() << "restart";

    m_measure = true;
    m_delay = true;
    m_timer->start(m_duration);
    measure();
}

void Device::clear()
{
    qDebug() << "clear";

    m_samples.clear();
    emit samplesChanged(m_samples);
}

Device::Coupling Device::ch1Coupling() const
{
    return m_ch1Coupling;
}

void Device::setCh1Coupling(Coupling coupling)
{
    if (m_ch1Coupling != coupling)
    {
        m_ch1Coupling = coupling;
        emit ch1CouplingChanged(m_ch1Coupling);
    }
}

Device::Probe Device::ch1Probe() const
{
    return m_ch1Probe;
}

void Device::setCh1Probe(Probe probe)
{
    if (m_ch1Probe != probe)
    {
        m_ch1Probe = probe;
        emit ch1ProbeChanged(m_ch1Probe);
        emit ch1DescriptionChanged(description(m_ch1Threshold, m_ch1Probe));
    }
}

int Device::ch1Threshold() const
{
    return m_ch1Threshold;
}

void Device::setCh1Threshold(int threshold)
{
    threshold = qBound(0, threshold, 255);
    if (m_ch1Threshold != threshold)
    {
        m_ch1Threshold = threshold;
        emit ch1ThresholdChanged(m_ch1Threshold);
        emit ch1DescriptionChanged(description(m_ch1Threshold, m_ch1Probe));
    }
}

Device::Coupling Device::ch2Coupling() const
{
    return m_ch2Coupling;
}

void Device::setCh2Coupling(Coupling coupling)
{
    if (m_ch2Coupling != coupling)
    {
        m_ch2Coupling = coupling;
        emit ch2CouplingChanged(m_ch2Coupling);
    }
}

Device::Probe Device::ch2Probe() const
{
    return m_ch2Probe;
}

void Device::setCh2Probe(Probe probe)
{
    if (m_ch2Probe != probe)
    {
        m_ch2Probe = probe;
        emit ch2ProbeChanged(m_ch2Probe);
        emit ch2DescriptionChanged(description(m_ch2Threshold, m_ch2Probe));
    }
}

int Device::ch2Threshold() const
{
    return m_ch2Threshold;
}

void Device::setCh2Threshold(int threshold)
{
    threshold = qBound(0, threshold, 255);
    if (m_ch2Threshold != threshold)
    {
        m_ch2Threshold = threshold;
        emit ch2ThresholdChanged(m_ch2Threshold);
        emit ch2DescriptionChanged(description(m_ch2Threshold, m_ch2Probe));
    }
}

Device::Clock Device::clock() const
{
    return m_clock;
}

void Device::setClock(Clock clock)
{
    if (m_clock != clock)
    {
        m_clock = clock;
        emit clockChanged(m_clock);

        clearThenRestart();
    }
}

qreal Device::clockFrequency() const
{
    return m_clockFrequency;
}

void Device::setClockFrequency(qreal frequency)
{
    frequency = qBound(9000000.0, frequency, 11000000.0);
    if (m_clockFrequency != frequency)
    {
        m_clockFrequency = frequency;
        emit clockFrequencyChanged(m_clockFrequency);

        clearThenRestart();
    }
}

Device::Trigger Device::trigger() const
{
    return m_trigger;
}

void Device::setTrigger(Trigger trigger)
{
    if (m_trigger != trigger)
    {
        m_trigger = trigger;
        emit triggerChanged(m_trigger);
    }
}

Device::Mode Device::mode() const
{
    return m_mode;
}

void Device::setMode(Mode mode)
{
    if (m_mode != mode)
    {
        switch (m_mode = mode)
        {
        case TimeMode:
            setCountEventEnabled(false);
            setStartStopEventEnabled(true);
            break;

        case FrequencyMode:
        case PeriodMode:
        case CountMode:
        case DutyMode:
            setCountEventEnabled(true);
            setStartStopEventEnabled(false);
            break;

        case GatePeriodMode:
        case GateFrequencyMode:
        case GateCountMode:
            setCountEventEnabled(true);
            setStartStopEventEnabled(true);
            break;

        default:
            break;
        }

        emit modeChanged(m_mode);

        clearThenRestart();
    }
}

Device::Event Device::countEvent() const
{
    return m_countEvent;
}

void Device::setCountEvent(Event event)
{
    if (m_countEvent != event)
    {
        m_countEvent = event;
        emit countEventChanged(m_countEvent);

        clearThenRestart();
    }
}

Device::Event Device::startEvent() const
{
    return m_startEvent;
}

void Device::setStartEvent(Event event)
{
    if (m_startEvent != event)
    {
        m_startEvent = event;
        emit startEventChanged(m_startEvent);

        clearThenRestart();
    }
}

Device::Event Device::stopEvent() const
{
    return m_stopEvent;
}

void Device::setStopEvent(Event event)
{
    if (m_stopEvent != event)
    {
        m_stopEvent = event;
        emit stopEventChanged(m_stopEvent);

        clearThenRestart();
    }
}

int Device::duration() const
{
    return m_duration;
}

void Device::setDuration(int duration)
{
    duration = qBound(0, duration, 10000);
    if (m_duration != duration)
    {
        m_duration = duration;
        emit durationChanged(m_duration);

        clearThenRestart();
    }
}

int Device::maxSamplesCount() const
{
    return m_maxSamplesCount;
}

void Device::setMaxSamplesCount(int count)
{
    count = qBound(5, count, 50);
    if (m_maxSamplesCount != count)
    {
        m_maxSamplesCount = count;
        emit maxSamplesCountChanged(m_maxSamplesCount);

        //....
    }
}

QList<qreal> Device::samples() const
{
    return m_samples;
}

void Device::complete(bool valid, qreal value)
{
    m_samples.enqueue(valid ? value : qQNaN());
    while (m_samples.size() > m_maxSamplesCount)
        m_samples.dequeue();

    emit samplesChanged(m_samples);

    m_measure = false;
    if (m_trigger == AutoTrigger && !m_delay)
        restart();
}

void Device::timeout()
{
    m_delay = false;
    if (m_trigger == AutoTrigger && !m_measure)
        restart();
}

void Device::clearThenRestart()
{
    clear();

    if (m_measure)
        restart();
}

void Device::setCountEventEnabled(bool enabled)
{
    if (m_countEventEnabled != enabled)
    {
        m_countEventEnabled = enabled;
        emit countEventEnabled(m_countEventEnabled);
    }
}

void Device::setStartStopEventEnabled(bool enabled)
{
    if (m_startStopEventEnabled != enabled)
    {
        m_startStopEventEnabled = enabled;
        emit startStopEventEnabled(m_startStopEventEnabled);
    }
}

QString Device::description(int threshold, Device::Probe probe) const
{
    switch (probe)
    {
    case x1Probe:
        return tr("%1").arg(-5.0 + 10.0 * threshold / 256, 0, 'f', 2, QChar(' '));

    case x10Probe:
        return tr("%1").arg(-50.0 + 100.0 * threshold / 256, 0, 'f', 1, QChar(' '));

    case x100Probe:
        return tr("%1").arg(-500.0 + 1000.0 * threshold / 256, 0, 'f', 0, QChar(' '));

    default:
        break;
    }

    return tr("---");
}


