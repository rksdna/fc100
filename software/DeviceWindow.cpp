#include <QGridLayout>
#include <QFontDatabase>
#include <QPushButton>
#include "Device.h"
#include "CustomDial.h"
#include "DeviceWindow.h"
#include "CustomButton.h"

DeviceWindow::DeviceWindow(QWidget *parent)
    : QWidget(parent),
      m_device(new Device(this))
{
    const QFont font = QFontDatabase::systemFont(QFontDatabase::FixedFont);

    CustomButton * const ch1CouplingButton = new CustomButton(tr("INPUT.1"));
    ch1CouplingButton->setFont(font);
    ch1CouplingButton->addValue(tr("DC"), Device::DcCoupling);
    ch1CouplingButton->addValue(tr("AC"), Device::AcCoupling);
    connect(ch1CouplingButton, reinterpret_cast<void (CustomButton::*)(Device::Coupling)>(&CustomButton::valueChanged), m_device, &Device::setCh1Coupling);
    connect(m_device, &Device::ch1CouplingChanged, ch1CouplingButton, &CustomButton::setValue);

    CustomButton * const ch1ProbeButton = new CustomButton(tr("PROBE.1"));
    ch1ProbeButton->setFont(font);
    ch1ProbeButton->addValue(tr("1:1"), Device::x1Probe);
    ch1ProbeButton->addValue(tr("1:10"), Device::x10Probe);
    ch1ProbeButton->addValue(tr("1:100"), Device::x100Probe);
    connect(ch1ProbeButton, reinterpret_cast<void (CustomButton::*)(Device::Probe)>(&CustomButton::valueChanged), m_device, &Device::setCh1Probe);
    connect(m_device, &Device::ch1ProbeChanged, ch1ProbeButton, &CustomButton::setValue);

    CustomDial * const ch1ThresholdDial = new CustomDial(tr("LEVEL.1"));
    ch1ThresholdDial->setFont(font);
    ch1ThresholdDial->setRange(0, 255);
    ch1ThresholdDial->setValue(128);
    connect(ch1ThresholdDial, &CustomDial::valueChanged, m_device, &Device::setCh1Threshold);
    connect(m_device, &Device::ch1ThresholdChanged, ch1ThresholdDial, &CustomDial::setValue);
    connect(m_device, &Device::ch1DescriptionChanged, ch1ThresholdDial, &CustomDial::setDescription);

    CustomButton * const ch2CouplingButton = new CustomButton(tr("INPUT.2"));
    ch2CouplingButton->setFont(font);
    ch2CouplingButton->addValue(tr("DC"), Device::DcCoupling);
    ch2CouplingButton->addValue(tr("AC"), Device::AcCoupling);
    connect(ch2CouplingButton, reinterpret_cast<void (CustomButton::*)(Device::Coupling)>(&CustomButton::valueChanged), m_device, &Device::setCh2Coupling);
    connect(m_device, &Device::ch2CouplingChanged, ch2CouplingButton, &CustomButton::setValue);

    CustomButton * const ch2ProbeButton = new CustomButton(tr("PROBE.2"));
    ch2ProbeButton->setFont(font);
    ch2ProbeButton->addValue(tr("1:1"), Device::x1Probe);
    ch2ProbeButton->addValue(tr("1:10"), Device::x10Probe);
    ch2ProbeButton->addValue(tr("1:100"), Device::x100Probe);
    connect(ch2ProbeButton, reinterpret_cast<void (CustomButton::*)(Device::Probe)>(&CustomButton::valueChanged), m_device, &Device::setCh2Probe);
    connect(m_device, &Device::ch2ProbeChanged, ch2ProbeButton, &CustomButton::setValue);

    CustomDial * const ch2ThresholdDial = new CustomDial(tr("LEVEL.2"));
    ch2ThresholdDial->setFont(font);
    ch2ThresholdDial->setRange(0, 255);
    ch2ThresholdDial->setValue(128);
    connect(ch2ThresholdDial, &CustomDial::valueChanged, m_device, &Device::setCh2Threshold);
    connect(m_device, &Device::ch2ThresholdChanged, ch2ThresholdDial, &CustomDial::setValue);
    connect(m_device, &Device::ch2DescriptionChanged, ch2ThresholdDial, &CustomDial::setDescription);

    CustomButton * const clockButton = new CustomButton(tr("REF"));
    clockButton->setFont(font);
    clockButton->addValue(tr("INT"), Device::InternalClock);
    clockButton->addValue(tr("EXT"), Device::ExternalClock);
    connect(clockButton, reinterpret_cast<void (CustomButton::*)(Device::Clock)>(&CustomButton::valueChanged), m_device, &Device::setClock);
    connect(m_device, &Device::clockChanged, clockButton, &CustomButton::setValue);

    CustomButton * const triggerButton = new CustomButton(tr("TRIGGER"));
    triggerButton->setFont(font);
    triggerButton->addValue(tr("AUTO"), Device::AutoTrigger);
    triggerButton->addValue(tr("MANUAL"), Device::ManualTrigger);
    connect(triggerButton, reinterpret_cast<void (CustomButton::*)(Device::Trigger)>(&CustomButton::valueChanged), m_device, &Device::setTrigger);
    connect(m_device, &Device::triggerChanged, triggerButton, &CustomButton::setValue);

    CustomButton * const modeButton = new CustomButton(tr("MODE"));
    modeButton->setFont(font);
    modeButton->addValue(tr("TIME"), Device::TimeMode);
    modeButton->addValue(tr("FREQUENCY"), Device::FrequencyMode);
    modeButton->addValue(tr("PERIOD"), Device::PeriodMode);
    modeButton->addValue(tr("COUNTER"), Device::CountMode);
    modeButton->addValue(tr("DUTY CYCLE"), Device::DutyMode);
    modeButton->addValue(tr("G FREQUENCY"), Device::GateFrequencyMode);
    modeButton->addValue(tr("G PERIOD"), Device::GatePeriodMode);
    modeButton->addValue(tr("G COUNTER"), Device::GateCountMode);
    connect(modeButton, reinterpret_cast<void (CustomButton::*)(Device::Mode)>(&CustomButton::valueChanged), m_device, &Device::setMode);
    connect(m_device, &Device::modeChanged, modeButton, &CustomButton::setValue);

    CustomButton * const countEventButton = new CustomButton(tr("INPUT"));
    countEventButton->setFont(font);
    countEventButton->addValue(tr("CH1 R"), Device::Ch1RisingEdgeEvent);
    countEventButton->addValue(tr("CH1 F"), Device::Ch1FallingEdgeEvent);
    countEventButton->addValue(tr("CH2 R"), Device::Ch2RisingEdgeEvent);
    countEventButton->addValue(tr("CH2 F"), Device::Ch2FallingEdgeEvent);
    connect(countEventButton, reinterpret_cast<void (CustomButton::*)(Device::Event)>(&CustomButton::valueChanged), m_device, &Device::setCountEvent);
    connect(m_device, &Device::countEventChanged, countEventButton, &CustomButton::setValue);
    connect(m_device, &Device::countEventEnabled, countEventButton, &CustomButton::setEnabled);

    CustomButton * const startEventButton = new CustomButton(tr("START ON"));
    startEventButton->setFont(font);
    startEventButton->addValue(tr("CH1 R"), Device::Ch1RisingEdgeEvent);
    startEventButton->addValue(tr("CH1 F"), Device::Ch1FallingEdgeEvent);
    startEventButton->addValue(tr("CH2 R"), Device::Ch2RisingEdgeEvent);
    startEventButton->addValue(tr("CH2 F"), Device::Ch2FallingEdgeEvent);
    connect(startEventButton, reinterpret_cast<void (CustomButton::*)(Device::Event)>(&CustomButton::valueChanged), m_device, &Device::setStartEvent);
    connect(m_device, &Device::startEventChanged, startEventButton, &CustomButton::setValue);
    connect(m_device, &Device::startStopEventEnabled, startEventButton, &CustomButton::setEnabled);

    CustomButton * const stopEventButton = new CustomButton(tr("STOP ON"));
    stopEventButton->setFont(font);
    stopEventButton->addValue(tr("CH1 R"), Device::Ch1RisingEdgeEvent);
    stopEventButton->addValue(tr("CH1 F"), Device::Ch1FallingEdgeEvent);
    stopEventButton->addValue(tr("CH2 R"), Device::Ch2RisingEdgeEvent);
    stopEventButton->addValue(tr("CH2 F"), Device::Ch2FallingEdgeEvent);
    connect(stopEventButton, reinterpret_cast<void (CustomButton::*)(Device::Event)>(&CustomButton::valueChanged), m_device, &Device::setStopEvent);
    connect(m_device, &Device::stopEventChanged, stopEventButton, &CustomButton::setValue);
    connect(m_device, &Device::startStopEventEnabled, stopEventButton, &CustomButton::setEnabled);

    QPushButton * const runButton = new QPushButton(tr("RUN"));
    runButton->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    runButton->setFont(font);

    CustomButton * const durationButton = new CustomButton(tr("TIME"));
    durationButton->setFont(font);
    durationButton->addValue(tr("5 mS"), 5);
    durationButton->addValue(tr("10 mS"), 10);
    durationButton->addValue(tr("50 mS"), 50);
    durationButton->addValue(tr("100 mS"), 100);
    durationButton->addValue(tr("500 mS"), 500);
    durationButton->addValue(tr("1 S"), 1000);
    durationButton->addValue(tr("5 S"), 5000);
    durationButton->addValue(tr("10 S"), 10000);
    connect(durationButton, &CustomButton::valueChanged, m_device, &Device::setDuration);
    connect(m_device, &Device::durationChanged, durationButton, &CustomButton::setValue);

    QGridLayout * const layout = new QGridLayout(this);

    layout->addWidget(ch1CouplingButton, 0, 0);
    layout->addWidget(ch1ProbeButton, 1, 0);
    layout->addWidget(ch1ThresholdDial, 0, 1, 2, 2);

    layout->addWidget(ch2CouplingButton, 0, 10);
    layout->addWidget(ch2ProbeButton, 1, 10);
    layout->addWidget(ch2ThresholdDial, 0, 8, 2, 2);

    layout->addWidget(triggerButton, 0, 4);
    layout->addWidget(runButton, 0, 5);
    layout->addWidget(clockButton, 0, 6);
    layout->addWidget(durationButton, 0, 7);

    layout->addWidget(modeButton, 1, 4);
    layout->addWidget(countEventButton, 1, 5);
    layout->addWidget(startEventButton, 1, 6);
    layout->addWidget(stopEventButton, 1, 7);

    m_device->reset();
}


