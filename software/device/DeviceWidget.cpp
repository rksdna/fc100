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
    DeviceChannel * const channel1 = device->channel1();
    DeviceChannel * const channel2 = device->channel2();
    DeviceProcessor * const processor = device->processor();
    DeviceController * const controller = device->controller();

    setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));
    setEnabled(device->isReady());
    connect(device, &Device::readyChanged, this, &DeviceWidget::setEnabled);

    CustomOptionButton * const ch1CouplingButton = new CustomOptionButton(tr("COUPLING.B"));
    ch1CouplingButton->addValueOption(tr("DC"), DeviceChannel::DcCoupling);
    ch1CouplingButton->addValueOption(tr("AC"), DeviceChannel::AcCoupling);
    ch1CouplingButton->setValue(channel1->coupling());
    ch1CouplingButton->setColor(controller->color());
    connect(controller, &DeviceController::colorChanged, ch1CouplingButton, &CustomButton::setColor);
    connect(ch1CouplingButton, reinterpret_cast<void (CustomOptionButton::*)(DeviceChannel::Coupling)>(&CustomOptionButton::valueChanged), channel1, &DeviceChannel::setCoupling);
    connect(channel1, &DeviceChannel::couplingChanged, ch1CouplingButton, &CustomOptionButton::setValue);

    CustomOptionButton * const ch1ProbeButton = new CustomOptionButton(tr("PROBE.B"));
    ch1ProbeButton->addValueOption(tr("1:1"), DeviceChannel::x1Probe);
    ch1ProbeButton->addValueOption(tr("1:10"), DeviceChannel::x10Probe);
    ch1ProbeButton->addValueOption(tr("1:100"), DeviceChannel::x100Probe);
    ch1ProbeButton->setValue(channel1->probe());
    ch1ProbeButton->setColor(controller->color());
    connect(controller, &DeviceController::colorChanged, ch1ProbeButton, &CustomButton::setColor);
    connect(ch1ProbeButton, reinterpret_cast<void (CustomOptionButton::*)(DeviceChannel::Probe)>(&CustomOptionButton::valueChanged), channel1, &DeviceChannel::setProbe);
    connect(channel1, &DeviceChannel::probeChanged, ch1ProbeButton, &CustomOptionButton::setValue);

    CustomDial * const ch1ThresholdDial = new CustomDial(tr("LEVEL.B"));
    ch1ThresholdDial->setRange(DeviceChannel::minThreshold(), DeviceChannel::maxThreshold());
    ch1ThresholdDial->setValue(channel1->threshold());
    ch1ThresholdDial->setText(channel1->text());
    ch1ThresholdDial->setColor(controller->color());
    connect(controller, &DeviceController::colorChanged, ch1ThresholdDial, &CustomDial::setColor);
    connect(ch1ThresholdDial, &CustomDial::valueChanged, channel1, &DeviceChannel::setThreshold);
    connect(channel1, &DeviceChannel::thresholdChanged, ch1ThresholdDial, &CustomDial::setValue);
    connect(channel1, &DeviceChannel::textChanged, ch1ThresholdDial, &CustomDial::setText);

    CustomOptionButton * const ch2CouplingButton = new CustomOptionButton(tr("COUPLING.A"));
    ch2CouplingButton->addValueOption(tr("DC"), DeviceChannel::DcCoupling);
    ch2CouplingButton->addValueOption(tr("AC"), DeviceChannel::AcCoupling);
    ch2CouplingButton->setValue(channel2->coupling());
    ch2CouplingButton->setColor(controller->color());
    connect(controller, &DeviceController::colorChanged, ch2CouplingButton, &CustomButton::setColor);
    connect(ch2CouplingButton, reinterpret_cast<void (CustomOptionButton::*)(DeviceChannel::Coupling)>(&CustomOptionButton::valueChanged), channel2, &DeviceChannel::setCoupling);
    connect(channel2, &DeviceChannel::couplingChanged, ch2CouplingButton, &CustomOptionButton::setValue);

    CustomOptionButton * const ch2ProbeButton = new CustomOptionButton(tr("PROBE.A"));
    ch2ProbeButton->addValueOption(tr("1:1"), DeviceChannel::x1Probe);
    ch2ProbeButton->addValueOption(tr("1:10"), DeviceChannel::x10Probe);
    ch2ProbeButton->addValueOption(tr("1:100"), DeviceChannel::x100Probe);
    ch2ProbeButton->setValue(channel2->probe());
    ch2ProbeButton->setColor(controller->color());
    connect(controller, &DeviceController::colorChanged, ch2ProbeButton, &CustomButton::setColor);
    connect(ch2ProbeButton, reinterpret_cast<void (CustomOptionButton::*)(DeviceChannel::Probe)>(&CustomOptionButton::valueChanged), channel2, &DeviceChannel::setProbe);
    connect(channel2, &DeviceChannel::probeChanged, ch2ProbeButton, &CustomOptionButton::setValue);

    CustomDial * const ch2ThresholdDial = new CustomDial(tr("LEVEL.A"));
    ch2ThresholdDial->setRange(DeviceChannel::minThreshold(), DeviceChannel::maxThreshold());
    ch2ThresholdDial->setValue(channel2->threshold());
    ch2ThresholdDial->setText(channel2->text());
    ch2ThresholdDial->setColor(controller->color());
    connect(controller, &DeviceController::colorChanged, ch2ThresholdDial, &CustomDial::setColor);
    connect(ch2ThresholdDial, &CustomDial::valueChanged, channel2, &DeviceChannel::setThreshold);
    connect(channel2, &DeviceChannel::thresholdChanged, ch2ThresholdDial, &CustomDial::setValue);
    connect(channel2, &DeviceChannel::textChanged, ch2ThresholdDial, &CustomDial::setText);

    CustomPushButton * const restartButton = new CustomPushButton(tr("START"));
    restartButton->setColor(controller->color());
    connect(controller, &DeviceController::colorChanged, restartButton, &CustomButton::setColor);
    connect(restartButton, &CustomPushButton::clicked, device, &Device::start);

    CustomPushButton * const clearButton = new CustomPushButton(tr("CLEAR"));
    clearButton->setColor(controller->color());
    connect(controller, &DeviceController::colorChanged, clearButton, &CustomButton::setColor);
    connect(clearButton, &CustomPushButton::clicked, processor, &DeviceProcessor::clear);

    CustomOptionButton * const conversionButton = new CustomOptionButton(tr("CONV."));
    conversionButton->addValueOption(tr("NONE"), DeviceProcessor::NoConversion);
    conversionButton->addValueOption(tr("EXPR."), DeviceProcessor::ExpressionConversion);
    conversionButton->setValue(processor->conversion());
    conversionButton->setColor(controller->color());
    connect(controller, &DeviceController::colorChanged, conversionButton, &CustomButton::setColor);
    connect(conversionButton, reinterpret_cast<void (CustomOptionButton::*)(DeviceProcessor::Conversion)>(&CustomOptionButton::valueChanged), processor, &DeviceProcessor::setConversion);
    connect(processor, &DeviceProcessor::conversionChanged, conversionButton, &CustomOptionButton::setValue);

    CustomOptionButton * const triggerButton = new CustomOptionButton(tr("TRIGGER"));
    triggerButton->addValueOption(tr("AUTO"), DeviceController::AutoTrigger);
    triggerButton->addValueOption(tr("MANUAL"), DeviceController::ManualTrigger);
    triggerButton->setValue(controller->trigger());
    triggerButton->setColor(controller->color());
    connect(controller, &DeviceController::colorChanged, triggerButton, &CustomButton::setColor);
    connect(triggerButton, reinterpret_cast<void (CustomOptionButton::*)(DeviceController::Trigger)>(&CustomOptionButton::valueChanged), controller, &DeviceController::setTrigger);
    connect(controller, &DeviceController::triggerChanged, triggerButton, &CustomOptionButton::setValue);

    CustomOptionButton * const modeButton = new CustomOptionButton(tr("MODE"));
    modeButton->addValueOption(tr("TIME"), DeviceController::TimeMode);
    modeButton->addValueOption(tr("FREQUENCY"), DeviceController::FrequencyMode);
    modeButton->addValueOption(tr("PERIOD"), DeviceController::PeriodMode);
    modeButton->addValueOption(tr("COUNTER"), DeviceController::CountMode);
    modeButton->addValueOption(tr("DUTY"), DeviceController::DutyMode);
    modeButton->addValueOption(tr("G. FREQ."), DeviceController::GateFrequencyMode);
    modeButton->addValueOption(tr("G. PERIOD"), DeviceController::GatePeriodMode);
    modeButton->addValueOption(tr("G. COUNTER"), DeviceController::GateCountMode);
    modeButton->setValue(controller->mode());
    modeButton->setColor(controller->color());
    connect(controller, &DeviceController::colorChanged, modeButton, &CustomButton::setColor);
    connect(modeButton, reinterpret_cast<void (CustomOptionButton::*)(DeviceController::Mode)>(&CustomOptionButton::valueChanged), controller, &DeviceController::setMode);
    connect(controller, &DeviceController::modeChanged, modeButton, &CustomOptionButton::setValue);

    CustomOptionButton * const countEventButton = new CustomOptionButton(tr("INPUT"));
    countEventButton->addValueOption(tr("CH.A L-H"), DeviceController::Ch2RisingEdgeEvent);
    countEventButton->addValueOption(tr("CH.A H-L"), DeviceController::Ch2FallingEdgeEvent);
    countEventButton->addValueOption(tr("CH.B L-H"), DeviceController::Ch1RisingEdgeEvent);
    countEventButton->addValueOption(tr("CH.B H-L"), DeviceController::Ch1FallingEdgeEvent);
    countEventButton->setValue(controller->countEvent());
    countEventButton->setColor(controller->color());
    countEventButton->setEnabled(controller->isCountEventEnabled());
    connect(controller, &DeviceController::colorChanged, countEventButton, &CustomButton::setColor);
    connect(countEventButton, reinterpret_cast<void (CustomOptionButton::*)(DeviceController::Event)>(&CustomOptionButton::valueChanged), controller, &DeviceController::setCountEvent);
    connect(controller, &DeviceController::countEventChanged, countEventButton, &CustomOptionButton::setValue);
    connect(controller, &DeviceController::countEventEnabled, countEventButton, &CustomOptionButton::setEnabled);

    CustomOptionButton * const startEventButton = new CustomOptionButton(tr("START ON"));
    startEventButton->addValueOption(tr("CH.A L-H"), DeviceController::Ch2RisingEdgeEvent);
    startEventButton->addValueOption(tr("CH.A H-L"), DeviceController::Ch2FallingEdgeEvent);
    startEventButton->addValueOption(tr("CH.B L-H"), DeviceController::Ch1RisingEdgeEvent);
    startEventButton->addValueOption(tr("CH.B H-L"), DeviceController::Ch1FallingEdgeEvent);
    startEventButton->setValue(controller->startEvent());
    startEventButton->setColor(controller->color());
    startEventButton->setEnabled(controller->isStartStopEventEnabled());
    connect(controller, &DeviceController::colorChanged, startEventButton, &CustomButton::setColor);
    connect(startEventButton, reinterpret_cast<void (CustomOptionButton::*)(DeviceController::Event)>(&CustomOptionButton::valueChanged), controller, &DeviceController::setStartEvent);
    connect(controller, &DeviceController::startEventChanged, startEventButton, &CustomOptionButton::setValue);
    connect(controller, &DeviceController::startStopEventEnabled, startEventButton, &CustomOptionButton::setEnabled);

    CustomOptionButton * const stopEventButton = new CustomOptionButton(tr("STOP ON"));    
    stopEventButton->addValueOption(tr("CH.A L-H"), DeviceController::Ch2RisingEdgeEvent);
    stopEventButton->addValueOption(tr("CH.A H-L"), DeviceController::Ch2FallingEdgeEvent);
    stopEventButton->addValueOption(tr("CH.B L-H"), DeviceController::Ch1RisingEdgeEvent);
    stopEventButton->addValueOption(tr("CH.B H-L"), DeviceController::Ch1FallingEdgeEvent);
    stopEventButton->setValue(controller->stopEvent());
    stopEventButton->setColor(controller->color());
    stopEventButton->setEnabled(controller->isStartStopEventEnabled());
    connect(controller, &DeviceController::colorChanged, stopEventButton, &CustomButton::setColor);
    connect(stopEventButton, reinterpret_cast<void (CustomOptionButton::*)(DeviceController::Event)>(&CustomOptionButton::valueChanged), controller, &DeviceController::setStopEvent);
    connect(controller, &DeviceController::stopEventChanged, stopEventButton, &CustomOptionButton::setValue);
    connect(controller, &DeviceController::startStopEventEnabled, stopEventButton, &CustomOptionButton::setEnabled);

    CustomOptionButton * const durationButton = new CustomOptionButton(tr("TIME"));
    durationButton->addValueOption(tr("5 mS"), 5);
    durationButton->addValueOption(tr("10 mS"), 10);
    durationButton->addValueOption(tr("50 mS"), 50);
    durationButton->addValueOption(tr("100 mS"), 100);
    durationButton->addValueOption(tr("500 mS"), 500);
    durationButton->addValueOption(tr("1 S"), 1000);
    durationButton->addValueOption(tr("5 S"), 5000);
    durationButton->addValueOption(tr("10 S"), 10000);
    durationButton->setValue(controller->duration());
    durationButton->setColor(controller->color());
    connect(controller, &DeviceController::colorChanged, durationButton, &CustomButton::setColor);
    connect(durationButton, &CustomOptionButton::valueChanged, controller, &DeviceController::setDuration);
    connect(controller, &DeviceController::durationChanged, durationButton, &CustomOptionButton::setValue);

    CustomTextDisplay * const minDisplay = new CustomTextDisplay(tr("MIN"));
    minDisplay->setColor(processor->color());
    connect(processor, &DeviceProcessor::colorChanged, minDisplay, &CustomDisplay::setColor);
    connect(processor, &DeviceProcessor::minChanged, minDisplay, &CustomTextDisplay::setText);

    CustomTextDisplay * const maxDisplay = new CustomTextDisplay(tr("MAX"));
    maxDisplay->setColor(processor->color());
    connect(processor, &DeviceProcessor::colorChanged, maxDisplay, &CustomDisplay::setColor);
    connect(processor, &DeviceProcessor::maxChanged, maxDisplay, &CustomTextDisplay::setText);

    CustomTextDisplay * const bandDisplay = new CustomTextDisplay(tr("MAX - MIN"));
    bandDisplay->setColor(processor->color());
    connect(processor, &DeviceProcessor::colorChanged, bandDisplay, &CustomDisplay::setColor);
    connect(processor, &DeviceProcessor::bandChanged, bandDisplay, &CustomTextDisplay::setText);

    CustomTextDisplay * const meanDisplay = new CustomTextDisplay(tr("MEAN"));
    meanDisplay->setColor(processor->color());
    connect(processor, &DeviceProcessor::colorChanged, meanDisplay, &CustomDisplay::setColor);
    connect(processor, &DeviceProcessor::meanChanged, meanDisplay, &CustomTextDisplay::setText);

    CustomTextDisplay * const standartDeviationDisplay = new CustomTextDisplay(tr("SD"));
    standartDeviationDisplay->setColor(processor->color());
    connect(processor, &DeviceProcessor::colorChanged, standartDeviationDisplay, &CustomDisplay::setColor);
    connect(processor, &DeviceProcessor::standartDeviationChanged, standartDeviationDisplay, &CustomTextDisplay::setText);

    CustomTextDisplay * const countDisplay = new CustomTextDisplay(tr("COUNT"));
    countDisplay->setColor(processor->color());
    connect(processor, &DeviceProcessor::colorChanged, countDisplay, &CustomDisplay::setColor);
    connect(processor, &DeviceProcessor::countChanged, countDisplay, &CustomTextDisplay::setText);

    CustomTrendDisplay * const trendDisplay = new CustomTrendDisplay;
    trendDisplay->setColor(processor->color());
    connect(processor, &DeviceProcessor::colorChanged, trendDisplay, &CustomDisplay::setColor);
    connect(processor, &DeviceProcessor::trendChanged, trendDisplay, &CustomTrendDisplay::setTrend);

    CustomBarDisplay * const barsDisplay = new CustomBarDisplay;
    barsDisplay->setColor(processor->color());
    connect(processor, &DeviceProcessor::colorChanged, barsDisplay, &CustomDisplay::setColor);
    connect(processor, &DeviceProcessor::barsChanged, barsDisplay, &CustomBarDisplay::setBars);

    CustomTextDisplay * const timeDisplay = new CustomTextDisplay(tr("TIME"));
    timeDisplay->setColor(processor->color());
    connect(processor, &DeviceProcessor::colorChanged, timeDisplay, &CustomDisplay::setColor);
    connect(processor, &DeviceProcessor::timeChanged, timeDisplay, &CustomTextDisplay::setText);

    CustomTextDisplay * const absoluteDisplay = new CustomTextDisplay(tr("ABS"));
    absoluteDisplay->setColor(processor->color());
    connect(processor, &DeviceProcessor::colorChanged, absoluteDisplay, &CustomDisplay::setColor);
    connect(processor, &DeviceProcessor::absoluteChanged, absoluteDisplay, &CustomTextDisplay::setText);

    CustomTextDisplay * const relativeDisplay = new CustomTextDisplay(tr("REL"));
    relativeDisplay->setColor(processor->color());
    connect(processor, &DeviceProcessor::colorChanged, relativeDisplay, &CustomDisplay::setColor);
    connect(processor, &DeviceProcessor::relativeChanged, relativeDisplay, &CustomTextDisplay::setText);

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

    layout->addWidget(ch2CouplingButton, 3, 0);
    layout->addWidget(ch2ProbeButton, 3, 1);
    layout->addWidget(triggerButton, 3, 2);
    layout->addWidget(restartButton, 3, 3);
    layout->addWidget(clearButton, 3, 4);
    layout->addWidget(ch1CouplingButton, 3, 5);
    layout->addWidget(ch1ProbeButton, 3, 6);

    layout->addWidget(ch2ThresholdDial, 4, 0, 2, 2);
    layout->addWidget(modeButton, 4, 2);
    layout->addWidget(durationButton, 4, 3);
    layout->addWidget(conversionButton, 4, 4);
    layout->addWidget(ch1ThresholdDial, 4, 5, 2, 2);

    layout->addWidget(countEventButton, 5, 2);
    layout->addWidget(startEventButton, 5, 3);
    layout->addWidget(stopEventButton, 5, 4);
}
