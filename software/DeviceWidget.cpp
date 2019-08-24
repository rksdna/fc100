#include <QGridLayout>
#include <QFontDatabase>
#include <QPushButton>
#include "Device.h"
#include "CustomDial.h"
#include "DeviceWidget.h"
#include "CustomButton.h"
#include "CustomDisplay.h"
#include "CommandButton.h"

DeviceWidget::DeviceWidget(Device *device, QWidget *parent)
    : QWidget(parent)
{
    setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));

    CommandButton * const restartButton = new CommandButton(tr("START"));
    connect(restartButton, &CommandButton::clicked, device, &Device::restart);

    CommandButton * const clearButton = new CommandButton(tr("CLEAR"));
    connect(clearButton, &CommandButton::clicked, device, &Device::clear);

    CustomButton * const ch1CouplingButton = new CustomButton(tr("INPUT.1"));
    ch1CouplingButton->addValue(tr("DC"), Device::DcCoupling);
    ch1CouplingButton->addValue(tr("AC"), Device::AcCoupling);
    connect(ch1CouplingButton, reinterpret_cast<void (CustomButton::*)(Device::Coupling)>(&CustomButton::valueChanged), device, &Device::setCh1Coupling);
    connect(device, &Device::ch1CouplingChanged, ch1CouplingButton, &CustomButton::setValue);

    CustomButton * const ch1ProbeButton = new CustomButton(tr("PROBE.1"));
    ch1ProbeButton->addValue(tr("1:1"), Device::x1Probe);
    ch1ProbeButton->addValue(tr("1:10"), Device::x10Probe);
    ch1ProbeButton->addValue(tr("1:100"), Device::x100Probe);
    connect(ch1ProbeButton, reinterpret_cast<void (CustomButton::*)(Device::Probe)>(&CustomButton::valueChanged), device, &Device::setCh1Probe);
    connect(device, &Device::ch1ProbeChanged, ch1ProbeButton, &CustomButton::setValue);

    CustomDial * const ch1ThresholdDial = new CustomDial(tr("LEVEL.1"));
    ch1ThresholdDial->setRange(0, 255);
    ch1ThresholdDial->setValue(128);
    connect(ch1ThresholdDial, &CustomDial::valueChanged, device, &Device::setCh1Threshold);
    connect(device, &Device::ch1ThresholdChanged, ch1ThresholdDial, &CustomDial::setValue);
    connect(device, &Device::ch1DescriptionChanged, ch1ThresholdDial, &CustomDial::setDescription);

    CustomButton * const ch2CouplingButton = new CustomButton(tr("INPUT.2"));
    ch2CouplingButton->addValue(tr("DC"), Device::DcCoupling);
    ch2CouplingButton->addValue(tr("AC"), Device::AcCoupling);
    connect(ch2CouplingButton, reinterpret_cast<void (CustomButton::*)(Device::Coupling)>(&CustomButton::valueChanged), device, &Device::setCh2Coupling);
    connect(device, &Device::ch2CouplingChanged, ch2CouplingButton, &CustomButton::setValue);

    CustomButton * const ch2ProbeButton = new CustomButton(tr("PROBE.2"));
    ch2ProbeButton->addValue(tr("1:1"), Device::x1Probe);
    ch2ProbeButton->addValue(tr("1:10"), Device::x10Probe);
    ch2ProbeButton->addValue(tr("1:100"), Device::x100Probe);
    connect(ch2ProbeButton, reinterpret_cast<void (CustomButton::*)(Device::Probe)>(&CustomButton::valueChanged), device, &Device::setCh2Probe);
    connect(device, &Device::ch2ProbeChanged, ch2ProbeButton, &CustomButton::setValue);

    CustomDial * const ch2ThresholdDial = new CustomDial(tr("LEVEL.2"));
    ch2ThresholdDial->setRange(0, 255);
    ch2ThresholdDial->setValue(128);
    connect(ch2ThresholdDial, &CustomDial::valueChanged, device, &Device::setCh2Threshold);
    connect(device, &Device::ch2ThresholdChanged, ch2ThresholdDial, &CustomDial::setValue);
    connect(device, &Device::ch2DescriptionChanged, ch2ThresholdDial, &CustomDial::setDescription);

    CustomButton * const clockButton = new CustomButton(tr("FUNC"));
    clockButton->addValue(tr("NO"), false);
    clockButton->addValue(tr("USER"), true);
    connect(clockButton, reinterpret_cast<void (CustomButton::*)(bool)>(&CustomButton::valueChanged), device, &Device::setFunctionEnabled);
    connect(device, &Device::functionEnabledChanged, clockButton, &CustomButton::setValue);

    CustomButton * const triggerButton = new CustomButton(tr("TRIGGER"));
    triggerButton->addValue(tr("AUTO"), Device::AutoTrigger);
    triggerButton->addValue(tr("MANUAL"), Device::ManualTrigger);
    connect(triggerButton, reinterpret_cast<void (CustomButton::*)(Device::Trigger)>(&CustomButton::valueChanged), device, &Device::setTrigger);
    connect(device, &Device::triggerChanged, triggerButton, &CustomButton::setValue);

    CustomButton * const modeButton = new CustomButton(tr("MODE"));
    modeButton->addValue(tr("TIME"), Device::TimeMode);
    modeButton->addValue(tr("FREQ"), Device::FrequencyMode);
    modeButton->addValue(tr("PERIOD"), Device::PeriodMode);
    modeButton->addValue(tr("COUNTER"), Device::CountMode);
    modeButton->addValue(tr("DUTY"), Device::DutyMode);
    modeButton->addValue(tr("G FREQ"), Device::GateFrequencyMode);
    modeButton->addValue(tr("G PERIOD"), Device::GatePeriodMode);
    modeButton->addValue(tr("G COUNTER"), Device::GateCountMode);
    connect(modeButton, reinterpret_cast<void (CustomButton::*)(Device::Mode)>(&CustomButton::valueChanged), device, &Device::setMode);
    connect(device, &Device::modeChanged, modeButton, &CustomButton::setValue);

    CustomButton * const countEventButton = new CustomButton(tr("INPUT"));
    countEventButton->addValue(tr("CH1 R"), Device::Ch1RisingEdgeEvent);
    countEventButton->addValue(tr("CH1 F"), Device::Ch1FallingEdgeEvent);
    countEventButton->addValue(tr("CH2 R"), Device::Ch2RisingEdgeEvent);
    countEventButton->addValue(tr("CH2 F"), Device::Ch2FallingEdgeEvent);
    connect(countEventButton, reinterpret_cast<void (CustomButton::*)(Device::Event)>(&CustomButton::valueChanged), device, &Device::setCountEvent);
    connect(device, &Device::countEventChanged, countEventButton, &CustomButton::setValue);
    connect(device, &Device::countEventEnabled, countEventButton, &CustomButton::setEnabled);

    CustomButton * const startEventButton = new CustomButton(tr("START ON"));
    startEventButton->addValue(tr("CH1 R"), Device::Ch1RisingEdgeEvent);
    startEventButton->addValue(tr("CH1 F"), Device::Ch1FallingEdgeEvent);
    startEventButton->addValue(tr("CH2 R"), Device::Ch2RisingEdgeEvent);
    startEventButton->addValue(tr("CH2 F"), Device::Ch2FallingEdgeEvent);
    connect(startEventButton, reinterpret_cast<void (CustomButton::*)(Device::Event)>(&CustomButton::valueChanged), device, &Device::setStartEvent);
    connect(device, &Device::startEventChanged, startEventButton, &CustomButton::setValue);
    connect(device, &Device::startStopEventEnabled, startEventButton, &CustomButton::setEnabled);

    CustomButton * const stopEventButton = new CustomButton(tr("STOP ON"));
    stopEventButton->addValue(tr("CH1 R"), Device::Ch1RisingEdgeEvent);
    stopEventButton->addValue(tr("CH1 F"), Device::Ch1FallingEdgeEvent);
    stopEventButton->addValue(tr("CH2 R"), Device::Ch2RisingEdgeEvent);
    stopEventButton->addValue(tr("CH2 F"), Device::Ch2FallingEdgeEvent);
    connect(stopEventButton, reinterpret_cast<void (CustomButton::*)(Device::Event)>(&CustomButton::valueChanged), device, &Device::setStopEvent);
    connect(device, &Device::stopEventChanged, stopEventButton, &CustomButton::setValue);
    connect(device, &Device::startStopEventEnabled, stopEventButton, &CustomButton::setEnabled);

    CustomButton * const durationButton = new CustomButton(tr("TIME"));
    durationButton->addValue(tr("5 mS"), 5);
    durationButton->addValue(tr("10 mS"), 10);
    durationButton->addValue(tr("50 mS"), 50);
    durationButton->addValue(tr("100 mS"), 100);
    durationButton->addValue(tr("500 mS"), 500);
    durationButton->addValue(tr("1 S"), 1000);
    durationButton->addValue(tr("5 S"), 5000);
    durationButton->addValue(tr("10 S"), 10000);
    connect(durationButton, &CustomButton::valueChanged, device, &Device::setDuration);
    connect(device, &Device::durationChanged, durationButton, &CustomButton::setValue);

    CustomDisplay * const display = new CustomDisplay;
    connect(device, &Device::sampleArr, display, &CustomDisplay::display1);
    connect(device, &Device::cleared, display, &CustomDisplay::clear);

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


