#include <QGridLayout>
#include <QFontDatabase>
#include <QPushButton>
#include "Device.h"
#include "CustomDial.h"
#include "DeviceWidget.h"
#include "DeviceChannel.h"
#include "CustomDisplay.h"
#include "CustomPushButton.h"
#include "CustomOptionButton.h"


DeviceWidget::DeviceWidget(Device *device, QWidget *parent)
    : QWidget(parent)
{
    setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));

    CustomOptionButton * const ch1CouplingButton = new CustomOptionButton(tr("INPUT.1"));
    ch1CouplingButton->addValueOption(tr("DC"), DeviceChannel::DcCoupling);
    ch1CouplingButton->addValueOption(tr("AC"), DeviceChannel::AcCoupling);
    connect(ch1CouplingButton, reinterpret_cast<void (CustomOptionButton::*)(DeviceChannel::Coupling)>(&CustomOptionButton::valueChanged), device->channel1(), &DeviceChannel::setCoupling);
    connect(device->channel1(), &DeviceChannel::couplingChanged, ch1CouplingButton, &CustomOptionButton::setValue);

    CustomOptionButton * const ch1ProbeButton = new CustomOptionButton(tr("PROBE.1"));
    ch1ProbeButton->addValueOption(tr("1:1"), DeviceChannel::x1Probe);
    ch1ProbeButton->addValueOption(tr("1:10"), DeviceChannel::x10Probe);
    ch1ProbeButton->addValueOption(tr("1:100"), DeviceChannel::x100Probe);
    connect(ch1ProbeButton, reinterpret_cast<void (CustomOptionButton::*)(DeviceChannel::Probe)>(&CustomOptionButton::valueChanged), device->channel1(), &DeviceChannel::setProbe);
    connect(device->channel1(), &DeviceChannel::probeChanged, ch1ProbeButton, &CustomOptionButton::setValue);

    CustomDial * const ch1ThresholdDial = new CustomDial(tr("LEVEL.1"));
    ch1ThresholdDial->setRange(DeviceChannel::minThreshold(), DeviceChannel::maxThreshold());
    connect(ch1ThresholdDial, &CustomDial::valueChanged, device->channel1(), &DeviceChannel::setThreshold);
    connect(device->channel1(), &DeviceChannel::thresholdChanged, ch1ThresholdDial, &CustomDial::setValue);
    connect(device->channel1(), &DeviceChannel::textChanged, ch1ThresholdDial, &CustomDial::setText);

    CustomOptionButton * const ch2CouplingButton = new CustomOptionButton(tr("INPUT.2"));
    ch2CouplingButton->addValueOption(tr("DC"), DeviceChannel::DcCoupling);
    ch2CouplingButton->addValueOption(tr("AC"), DeviceChannel::AcCoupling);
    connect(ch2CouplingButton, reinterpret_cast<void (CustomOptionButton::*)(DeviceChannel::Coupling)>(&CustomOptionButton::valueChanged), device->channel2(), &DeviceChannel::setCoupling);
    connect(device->channel2(), &DeviceChannel::couplingChanged, ch2CouplingButton, &CustomOptionButton::setValue);

    CustomOptionButton * const ch2ProbeButton = new CustomOptionButton(tr("PROBE.2"));
    ch2ProbeButton->addValueOption(tr("1:1"), DeviceChannel::x1Probe);
    ch2ProbeButton->addValueOption(tr("1:10"), DeviceChannel::x10Probe);
    ch2ProbeButton->addValueOption(tr("1:10"), DeviceChannel::x100Probe);
    connect(ch2ProbeButton, reinterpret_cast<void (CustomOptionButton::*)(DeviceChannel::Probe)>(&CustomOptionButton::valueChanged), device->channel2(), &DeviceChannel::setProbe);
    connect(device->channel2(), &DeviceChannel::probeChanged, ch2ProbeButton, &CustomOptionButton::setValue);

    CustomDial * const ch2ThresholdDial = new CustomDial(tr("LEVEL.2"));
    ch2ThresholdDial->setRange(DeviceChannel::minThreshold(), DeviceChannel::maxThreshold());
    connect(ch2ThresholdDial, &CustomDial::valueChanged, device->channel2(), &DeviceChannel::setThreshold);
    connect(device->channel2(), &DeviceChannel::thresholdChanged, ch2ThresholdDial, &CustomDial::setValue);
    connect(device->channel2(), &DeviceChannel::textChanged, ch2ThresholdDial, &CustomDial::setText);


    CustomPushButton * const restartButton = new CustomPushButton(tr("START"));
    connect(restartButton, &CustomPushButton::clicked, device, &Device::restart);

    CustomPushButton * const clearButton = new CustomPushButton(tr("CLEAR"));
    connect(clearButton, &CustomPushButton::clicked, device, &Device::clear);




    CustomOptionButton * const clockButton = new CustomOptionButton(tr("FUNC"));
    clockButton->addValueOption(tr("NO"), false);
    clockButton->addValueOption(tr("USER"), true);
    connect(clockButton, reinterpret_cast<void (CustomOptionButton::*)(bool)>(&CustomOptionButton::valueChanged), device, &Device::setFunctionEnabled);
    connect(device, &Device::functionEnabledChanged, clockButton, &CustomOptionButton::setValue);

    CustomOptionButton * const triggerButton = new CustomOptionButton(tr("TRIGGER"));
    triggerButton->addValueOption(tr("AUTO"), Device::AutoTrigger);
    triggerButton->addValueOption(tr("MANUAL"), Device::ManualTrigger);
    connect(triggerButton, reinterpret_cast<void (CustomOptionButton::*)(Device::Trigger)>(&CustomOptionButton::valueChanged), device, &Device::setTrigger);
    connect(device, &Device::triggerChanged, triggerButton, &CustomOptionButton::setValue);

    CustomOptionButton * const modeButton = new CustomOptionButton(tr("MODE"));
    modeButton->addValueOption(tr("TIME"), Device::TimeMode);
    modeButton->addValueOption(tr("FREQ"), Device::FrequencyMode);
    modeButton->addValueOption(tr("PERIOD"), Device::PeriodMode);
    modeButton->addValueOption(tr("COUNTER"), Device::CountMode);
    modeButton->addValueOption(tr("DUTY"), Device::DutyMode);
    modeButton->addValueOption(tr("G FREQ"), Device::GateFrequencyMode);
    modeButton->addValueOption(tr("G PERIOD"), Device::GatePeriodMode);
    modeButton->addValueOption(tr("G COUNTER"), Device::GateCountMode);
    connect(modeButton, reinterpret_cast<void (CustomOptionButton::*)(Device::Mode)>(&CustomOptionButton::valueChanged), device, &Device::setMode);
    connect(device, &Device::modeChanged, modeButton, &CustomOptionButton::setValue);

    CustomOptionButton * const countEventButton = new CustomOptionButton(tr("INPUT"));
    countEventButton->addValueOption(tr("CH1 R"), Device::Ch1RisingEdgeEvent);
    countEventButton->addValueOption(tr("CH1 F"), Device::Ch1FallingEdgeEvent);
    countEventButton->addValueOption(tr("CH2 R"), Device::Ch2RisingEdgeEvent);
    countEventButton->addValueOption(tr("CH2 F"), Device::Ch2FallingEdgeEvent);
    connect(countEventButton, reinterpret_cast<void (CustomOptionButton::*)(Device::Event)>(&CustomOptionButton::valueChanged), device, &Device::setCountEvent);
    connect(device, &Device::countEventChanged, countEventButton, &CustomOptionButton::setValue);
    connect(device, &Device::countEventEnabled, countEventButton, &CustomOptionButton::setEnabled);

    CustomOptionButton * const startEventButton = new CustomOptionButton(tr("START ON"));
    startEventButton->addValueOption(tr("CH1 R"), Device::Ch1RisingEdgeEvent);
    startEventButton->addValueOption(tr("CH1 F"), Device::Ch1FallingEdgeEvent);
    startEventButton->addValueOption(tr("CH2 R"), Device::Ch2RisingEdgeEvent);
    startEventButton->addValueOption(tr("CH2 F"), Device::Ch2FallingEdgeEvent);
    connect(startEventButton, reinterpret_cast<void (CustomOptionButton::*)(Device::Event)>(&CustomOptionButton::valueChanged), device, &Device::setStartEvent);
    connect(device, &Device::startEventChanged, startEventButton, &CustomOptionButton::setValue);
    connect(device, &Device::startStopEventEnabled, startEventButton, &CustomOptionButton::setEnabled);

    CustomOptionButton * const stopEventButton = new CustomOptionButton(tr("STOP ON"));
    stopEventButton->addValueOption(tr("CH1 R"), Device::Ch1RisingEdgeEvent);
    stopEventButton->addValueOption(tr("CH1 F"), Device::Ch1FallingEdgeEvent);
    stopEventButton->addValueOption(tr("CH2 R"), Device::Ch2RisingEdgeEvent);
    stopEventButton->addValueOption(tr("CH2 F"), Device::Ch2FallingEdgeEvent);
    connect(stopEventButton, reinterpret_cast<void (CustomOptionButton::*)(Device::Event)>(&CustomOptionButton::valueChanged), device, &Device::setStopEvent);
    connect(device, &Device::stopEventChanged, stopEventButton, &CustomOptionButton::setValue);
    connect(device, &Device::startStopEventEnabled, stopEventButton, &CustomOptionButton::setEnabled);

    CustomOptionButton * const durationButton = new CustomOptionButton(tr("TIME"));
    durationButton->addValueOption(tr("5 mS"), 5);
    durationButton->addValueOption(tr("10 mS"), 10);
    durationButton->addValueOption(tr("50 mS"), 50);
    durationButton->addValueOption(tr("100 mS"), 100);
    durationButton->addValueOption(tr("500 mS"), 500);
    durationButton->addValueOption(tr("1 S"), 1000);
    durationButton->addValueOption(tr("5 S"), 5000);
    durationButton->addValueOption(tr("10 S"), 10000);
    connect(durationButton, &CustomOptionButton::valueChanged, device, &Device::setDuration);
    connect(device, &Device::durationChanged, durationButton, &CustomOptionButton::setValue);

    CustomDisplay * const display = new CustomDisplay(tr("MIN"));

    QGridLayout * const layout = new QGridLayout(this);

    layout->addWidget(display, 0, 0, 1, 7);

    layout->addWidget(ch1CouplingButton, 1, 0);
    layout->addWidget(ch1ProbeButton, 1, 1);
    layout->addWidget(ch1ThresholdDial, 2, 0, 2, 2);

    layout->addWidget(ch2CouplingButton, 1, 5);
    layout->addWidget(ch2ProbeButton, 1, 6);
    layout->addWidget(ch2ThresholdDial, 2, 5, 2, 2);

    layout->addWidget(triggerButton, 1, 2);
    layout->addWidget(restartButton, 1, 3);
    layout->addWidget(clearButton, 1, 4);

    layout->addWidget(modeButton, 2, 2);
    layout->addWidget(durationButton, 2, 3);
    layout->addWidget(clockButton, 2, 4);

    layout->addWidget(countEventButton, 3, 2);
    layout->addWidget(startEventButton, 3, 3);
    layout->addWidget(stopEventButton, 3, 4);

}


