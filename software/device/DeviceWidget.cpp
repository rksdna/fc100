#include <QGridLayout>
#include <QFontDatabase>
#include "Device.h"
#include "CustomDial.h"
#include "DeviceWidget.h"
#include "DeviceChannel.h"
#include "DeviceProcessor.h"
#include "DeviceController.h"
#include "CustomPushButton.h"
#include "CustomBarDisplay.h"
#include "CustomTextDisplay.h"
#include "CustomOptionButton.h"
#include "CustomTrendDisplay.h"

DeviceWidget::DeviceWidget(Device *device, QWidget *parent)
    : QWidget(parent)
{
    setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));

    CustomOptionButton * const ch1CouplingButton = new CustomOptionButton(tr("INPUT.1"));
    ch1CouplingButton->addValueOption(tr("DC"), DeviceChannel::DcCoupling);
    ch1CouplingButton->addValueOption(tr("AC"), DeviceChannel::AcCoupling);
    ch1CouplingButton->setValue(device->channel1()->coupling());
    connect(ch1CouplingButton, reinterpret_cast<void (CustomOptionButton::*)(DeviceChannel::Coupling)>(&CustomOptionButton::valueChanged), device->channel1(), &DeviceChannel::setCoupling);
    connect(device->channel1(), &DeviceChannel::couplingChanged, ch1CouplingButton, &CustomOptionButton::setValue);

    CustomOptionButton * const ch1ProbeButton = new CustomOptionButton(tr("PROBE.1"));
    ch1ProbeButton->addValueOption(tr("1:1"), DeviceChannel::x1Probe);
    ch1ProbeButton->addValueOption(tr("1:10"), DeviceChannel::x10Probe);
    ch1ProbeButton->addValueOption(tr("1:100"), DeviceChannel::x100Probe);
    ch1ProbeButton->setValue(device->channel1()->probe());
    connect(ch1ProbeButton, reinterpret_cast<void (CustomOptionButton::*)(DeviceChannel::Probe)>(&CustomOptionButton::valueChanged), device->channel1(), &DeviceChannel::setProbe);
    connect(device->channel1(), &DeviceChannel::probeChanged, ch1ProbeButton, &CustomOptionButton::setValue);

    CustomDial * const ch1ThresholdDial = new CustomDial(tr("LEVEL.1"));
    ch1ThresholdDial->setRange(DeviceChannel::minThreshold(), DeviceChannel::maxThreshold());
    ch1ThresholdDial->setValue(device->channel1()->threshold());
    ch1ThresholdDial->setText(device->channel1()->text());
    connect(ch1ThresholdDial, &CustomDial::valueChanged, device->channel1(), &DeviceChannel::setThreshold);
    connect(device->channel1(), &DeviceChannel::thresholdChanged, ch1ThresholdDial, &CustomDial::setValue);
    connect(device->channel1(), &DeviceChannel::textChanged, ch1ThresholdDial, &CustomDial::setText);

    CustomOptionButton * const ch2CouplingButton = new CustomOptionButton(tr("INPUT.2"));
    ch2CouplingButton->addValueOption(tr("DC"), DeviceChannel::DcCoupling);
    ch2CouplingButton->addValueOption(tr("AC"), DeviceChannel::AcCoupling);
    ch2CouplingButton->setValue(device->channel2()->coupling());
    connect(ch2CouplingButton, reinterpret_cast<void (CustomOptionButton::*)(DeviceChannel::Coupling)>(&CustomOptionButton::valueChanged), device->channel2(), &DeviceChannel::setCoupling);
    connect(device->channel2(), &DeviceChannel::couplingChanged, ch2CouplingButton, &CustomOptionButton::setValue);

    CustomOptionButton * const ch2ProbeButton = new CustomOptionButton(tr("PROBE.2"));
    ch2ProbeButton->addValueOption(tr("1:1"), DeviceChannel::x1Probe);
    ch2ProbeButton->addValueOption(tr("1:10"), DeviceChannel::x10Probe);
    ch2ProbeButton->addValueOption(tr("1:10"), DeviceChannel::x100Probe);
    ch2ProbeButton->setValue(device->channel2()->probe());
    connect(ch2ProbeButton, reinterpret_cast<void (CustomOptionButton::*)(DeviceChannel::Probe)>(&CustomOptionButton::valueChanged), device->channel2(), &DeviceChannel::setProbe);
    connect(device->channel2(), &DeviceChannel::probeChanged, ch2ProbeButton, &CustomOptionButton::setValue);

    CustomDial * const ch2ThresholdDial = new CustomDial(tr("LEVEL.2"));
    ch2ThresholdDial->setRange(DeviceChannel::minThreshold(), DeviceChannel::maxThreshold());
    ch2ThresholdDial->setValue(device->channel2()->threshold());
    ch2ThresholdDial->setText(device->channel2()->text());
    connect(ch2ThresholdDial, &CustomDial::valueChanged, device->channel2(), &DeviceChannel::setThreshold);
    connect(device->channel2(), &DeviceChannel::thresholdChanged, ch2ThresholdDial, &CustomDial::setValue);
    connect(device->channel2(), &DeviceChannel::textChanged, ch2ThresholdDial, &CustomDial::setText);

    CustomPushButton * const restartButton = new CustomPushButton(tr("START"));
    connect(restartButton, &CustomPushButton::clicked, device, &Device::start);

    CustomPushButton * const clearButton = new CustomPushButton(tr("CLEAR"));
    connect(clearButton, &CustomPushButton::clicked, device->processor(), &DeviceProcessor::clear);

    CustomOptionButton * const conversionButton = new CustomOptionButton(tr("CONVERSION"));
    conversionButton->addValueOption(tr("NONE"), DeviceProcessor::NoConversion);
    conversionButton->addValueOption(tr("EXPRESSION"), DeviceProcessor::ExpressionConversion);
    conversionButton->setValue(device->processor()->conversion());
    connect(conversionButton, reinterpret_cast<void (CustomOptionButton::*)(DeviceProcessor::Conversion)>(&CustomOptionButton::valueChanged), device->processor(), &DeviceProcessor::setConversion);
    connect(device->processor(), &DeviceProcessor::conversionChanged, conversionButton, &CustomOptionButton::setValue);

    CustomOptionButton * const triggerButton = new CustomOptionButton(tr("TRIGGER"));
    triggerButton->addValueOption(tr("AUTO"), DeviceController::AutoTrigger);
    triggerButton->addValueOption(tr("MANUAL"), DeviceController::ManualTrigger);
    triggerButton->setValue(device->controller()->trigger());
    connect(triggerButton, reinterpret_cast<void (CustomOptionButton::*)(DeviceController::Trigger)>(&CustomOptionButton::valueChanged), device->controller(), &DeviceController::setTrigger);
    connect(device->controller(), &DeviceController::triggerChanged, triggerButton, &CustomOptionButton::setValue);

    CustomOptionButton * const modeButton = new CustomOptionButton(tr("MODE"));
    modeButton->addValueOption(tr("TIME"), DeviceController::TimeMode);
    modeButton->addValueOption(tr("FREQ"), DeviceController::FrequencyMode);
    modeButton->addValueOption(tr("PERIOD"), DeviceController::PeriodMode);
    modeButton->addValueOption(tr("COUNTER"), DeviceController::CountMode);
    modeButton->addValueOption(tr("DUTY"), DeviceController::DutyMode);
    modeButton->addValueOption(tr("G FREQ"), DeviceController::GateFrequencyMode);
    modeButton->addValueOption(tr("G PERIOD"), DeviceController::GatePeriodMode);
    modeButton->addValueOption(tr("G COUNTER"), DeviceController::GateCountMode);
    modeButton->setValue(device->controller()->mode());
    connect(modeButton, reinterpret_cast<void (CustomOptionButton::*)(DeviceController::Mode)>(&CustomOptionButton::valueChanged), device->controller(), &DeviceController::setMode);
    connect(device->controller(), &DeviceController::modeChanged, modeButton, &CustomOptionButton::setValue);

    CustomOptionButton * const countEventButton = new CustomOptionButton(tr("INPUT"));
    countEventButton->addValueOption(tr("CH1 R"), DeviceController::Ch1RisingEdgeEvent);
    countEventButton->addValueOption(tr("CH1 F"), DeviceController::Ch1FallingEdgeEvent);
    countEventButton->addValueOption(tr("CH2 R"), DeviceController::Ch2RisingEdgeEvent);
    countEventButton->addValueOption(tr("CH2 F"), DeviceController::Ch2FallingEdgeEvent);
    countEventButton->setValue(device->controller()->countEvent());
    connect(countEventButton, reinterpret_cast<void (CustomOptionButton::*)(DeviceController::Event)>(&CustomOptionButton::valueChanged), device->controller(), &DeviceController::setCountEvent);
    connect(device->controller(), &DeviceController::countEventChanged, countEventButton, &CustomOptionButton::setValue);
    connect(device->controller(), &DeviceController::countEventEnabled, countEventButton, &CustomOptionButton::setEnabled);

    CustomOptionButton * const startEventButton = new CustomOptionButton(tr("START ON"));
    startEventButton->addValueOption(tr("CH1 R"), DeviceController::Ch1RisingEdgeEvent);
    startEventButton->addValueOption(tr("CH1 F"), DeviceController::Ch1FallingEdgeEvent);
    startEventButton->addValueOption(tr("CH2 R"), DeviceController::Ch2RisingEdgeEvent);
    startEventButton->addValueOption(tr("CH2 F"), DeviceController::Ch2FallingEdgeEvent);
    startEventButton->setValue(device->controller()->startEvent());
    connect(startEventButton, reinterpret_cast<void (CustomOptionButton::*)(DeviceController::Event)>(&CustomOptionButton::valueChanged), device->controller(), &DeviceController::setStartEvent);
    connect(device->controller(), &DeviceController::startEventChanged, startEventButton, &CustomOptionButton::setValue);
    connect(device->controller(), &DeviceController::startStopEventEnabled, startEventButton, &CustomOptionButton::setEnabled);

    CustomOptionButton * const stopEventButton = new CustomOptionButton(tr("STOP ON"));
    stopEventButton->addValueOption(tr("CH1 R"), DeviceController::Ch1RisingEdgeEvent);
    stopEventButton->addValueOption(tr("CH1 F"), DeviceController::Ch1FallingEdgeEvent);
    stopEventButton->addValueOption(tr("CH2 R"), DeviceController::Ch2RisingEdgeEvent);
    stopEventButton->addValueOption(tr("CH2 F"), DeviceController::Ch2FallingEdgeEvent);
    stopEventButton->setValue(device->controller()->stopEvent());
    connect(stopEventButton, reinterpret_cast<void (CustomOptionButton::*)(DeviceController::Event)>(&CustomOptionButton::valueChanged), device->controller(), &DeviceController::setStopEvent);
    connect(device->controller(), &DeviceController::stopEventChanged, stopEventButton, &CustomOptionButton::setValue);
    connect(device->controller(), &DeviceController::startStopEventEnabled, stopEventButton, &CustomOptionButton::setEnabled);

    CustomOptionButton * const durationButton = new CustomOptionButton(tr("TIME"));
    durationButton->addValueOption(tr("5 mS"), 5);
    durationButton->addValueOption(tr("10 mS"), 10);
    durationButton->addValueOption(tr("50 mS"), 50);
    durationButton->addValueOption(tr("100 mS"), 100);
    durationButton->addValueOption(tr("500 mS"), 500);
    durationButton->addValueOption(tr("1 S"), 1000);
    durationButton->addValueOption(tr("5 S"), 5000);
    durationButton->addValueOption(tr("10 S"), 10000);
    durationButton->setValue(device->controller()->duration());
    connect(durationButton, &CustomOptionButton::valueChanged, device->controller(), &DeviceController::setDuration);
    connect(device->controller(), &DeviceController::durationChanged, durationButton, &CustomOptionButton::setValue);

    CustomTextDisplay * const minDisplay = new CustomTextDisplay(tr("MIN"));
    connect(device->processor(), &DeviceProcessor::minChanged, minDisplay, &CustomTextDisplay::setText);

    CustomTextDisplay * const maxDisplay = new CustomTextDisplay(tr("MAX"));
    connect(device->processor(), &DeviceProcessor::maxChanged, maxDisplay, &CustomTextDisplay::setText);

    CustomTextDisplay * const bandDisplay = new CustomTextDisplay(tr("MAX - MIN"));
    connect(device->processor(), &DeviceProcessor::bandChanged, bandDisplay, &CustomTextDisplay::setText);

    CustomTextDisplay * const meanDisplay = new CustomTextDisplay(tr("MEAN"));
    connect(device->processor(), &DeviceProcessor::meanChanged, meanDisplay, &CustomTextDisplay::setText);

    CustomTextDisplay * const standartDeviationDisplay = new CustomTextDisplay(tr("SD"));
    connect(device->processor(), &DeviceProcessor::standartDeviationChanged, standartDeviationDisplay, &CustomTextDisplay::setText);

    CustomTextDisplay * const countDisplay = new CustomTextDisplay(tr("COUNT"));
    connect(device->processor(), &DeviceProcessor::countChanged, countDisplay, &CustomTextDisplay::setText);

    CustomTrendDisplay * const trendDisplay = new CustomTrendDisplay;
    connect(device->processor(), &DeviceProcessor::trendChanged, trendDisplay, &CustomTrendDisplay::setTrend);

    CustomBarDisplay * const barsDisplay = new CustomBarDisplay;
    connect(device->processor(), &DeviceProcessor::barsChanged, barsDisplay, &CustomBarDisplay::setBars);

    CustomTextDisplay * const timeDisplay = new CustomTextDisplay(tr("TIME"));
    connect(device->processor(), &DeviceProcessor::timeChanged, timeDisplay, &CustomTextDisplay::setText);

    CustomTextDisplay * const absoluteDisplay = new CustomTextDisplay(tr("ABS"));
    connect(device->processor(), &DeviceProcessor::absoluteChanged, absoluteDisplay, &CustomTextDisplay::setText);

    CustomTextDisplay * const relativeDisplay = new CustomTextDisplay(tr("REL"));
    connect(device->processor(), &DeviceProcessor::relativeChanged, relativeDisplay, &CustomTextDisplay::setText);

    QGridLayout * const layout = new QGridLayout(this);

    layout->addWidget(minDisplay, 0, 0);
    layout->addWidget(maxDisplay, 0, 1);
    layout->addWidget(bandDisplay, 0, 2);
    layout->addWidget(meanDisplay, 0, 4);
    layout->addWidget(standartDeviationDisplay, 0, 5);
    layout->addWidget(countDisplay, 0, 6);

    layout->addWidget(trendDisplay, 1, 0, 1, 3);
    layout->addWidget(barsDisplay, 1, 4, 1, 3);

    layout->addWidget(timeDisplay, 2, 0, 1, 2);
    layout->addWidget(absoluteDisplay, 2, 2, 1, 3);
    layout->addWidget(relativeDisplay, 2, 5, 1, 2);

    layout->addWidget(ch1CouplingButton, 3, 0);
    layout->addWidget(ch1ProbeButton, 3, 1);
    layout->addWidget(triggerButton, 3, 2);
    layout->addWidget(restartButton, 3, 3);
    layout->addWidget(clearButton, 3, 4);
    layout->addWidget(ch2CouplingButton, 3, 5);
    layout->addWidget(ch2ProbeButton, 3, 6);

    layout->addWidget(ch1ThresholdDial, 4, 0, 2, 2);
    layout->addWidget(modeButton, 4, 2);
    layout->addWidget(durationButton, 4, 3);
    layout->addWidget(conversionButton, 4, 4);
    layout->addWidget(ch2ThresholdDial, 4, 5, 2, 2);

    layout->addWidget(countEventButton, 5, 2);
    layout->addWidget(startEventButton, 5, 3);
    layout->addWidget(stopEventButton, 5, 4);
}
