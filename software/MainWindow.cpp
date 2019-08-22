#include <QGridLayout>
#include <QFontDatabase>
#include "MainWindow.h"
#include "Device.h"
#include "CustomButton.h"
#include "CustomDial.h"

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent),
      m_device(new Device(this))
{
    const QFont font = QFontDatabase::systemFont(QFontDatabase::FixedFont);

    CustomButton * const ch1CouplingButton = new CustomButton(tr("CPL.1"));
    ch1CouplingButton->setFont(font);
    ch1CouplingButton->addValue(tr("DC"), Device::DcCoupling);
    ch1CouplingButton->addValue(tr("AC"), Device::AcCoupling);
    connect(ch1CouplingButton, reinterpret_cast<void (CustomButton::*)(Device::Coupling)>(&CustomButton::valueChanged), m_device, &Device::setCh1Coupling);
    connect(m_device, &Device::ch1CouplingChanged, ch1CouplingButton, &CustomButton::setValue);

    CustomButton * const ch2CouplingButton = new CustomButton(tr("CPL.2"));
    ch2CouplingButton->setFont(font);
    ch2CouplingButton->addValue(tr("DC"), Device::DcCoupling);
    ch2CouplingButton->addValue(tr("AC"), Device::AcCoupling);
    connect(ch2CouplingButton, reinterpret_cast<void (CustomButton::*)(Device::Coupling)>(&CustomButton::valueChanged), m_device, &Device::setCh2Coupling);
    connect(m_device, &Device::ch2CouplingChanged, ch2CouplingButton, &CustomButton::setValue);

    CustomButton * const ch1ProbeButton = new CustomButton(tr("ATT.1"));
    ch1ProbeButton->setFont(font);
    ch1ProbeButton->addValue(tr("1:1"), Device::x1Probe);
    ch1ProbeButton->addValue(tr("1:10"), Device::x10Probe);
    ch1ProbeButton->addValue(tr("1:100"), Device::x100Probe);
    connect(ch1ProbeButton, reinterpret_cast<void (CustomButton::*)(Device::Probe)>(&CustomButton::valueChanged), m_device, &Device::setCh1Probe);
    connect(m_device, &Device::ch1ProbeChanged, ch1ProbeButton, &CustomButton::setValue);

    CustomButton * const ch2ProbeButton = new CustomButton(tr("ATT.2"));
    ch2ProbeButton->setFont(font);
    ch2ProbeButton->addValue(tr("1:1"), Device::x1Probe);
    ch2ProbeButton->addValue(tr("1:10"), Device::x10Probe);
    ch2ProbeButton->addValue(tr("1:100"), Device::x100Probe);
    connect(ch2ProbeButton, reinterpret_cast<void (CustomButton::*)(Device::Probe)>(&CustomButton::valueChanged), m_device, &Device::setCh2Probe);
    connect(m_device, &Device::ch2ProbeChanged, ch2ProbeButton, &CustomButton::setValue);

    CustomButton * const clockButton = new CustomButton(tr("REF"));
    clockButton->setFont(font);
    clockButton->addValue(tr("INT"), Device::InternalClock);
    clockButton->addValue(tr("EXT"), Device::ExternalClock);
    connect(clockButton, reinterpret_cast<void (CustomButton::*)(Device::Clock)>(&CustomButton::valueChanged), m_device, &Device::setClock);
    connect(m_device, &Device::clockChanged, clockButton, &CustomButton::setValue);

    CustomDial * const ch1ThresholdDial = new CustomDial(tr("LVL.1"));
    ch1ThresholdDial->setFont(font);
    ch1ThresholdDial->setRange(0, 255);
    ch1ThresholdDial->setValue(128);
    connect(ch1ThresholdDial, &CustomDial::valueChanged, m_device, &Device::setCh1Threshold);
    connect(m_device, &Device::ch1ThresholdChanged, ch1ThresholdDial, &CustomDial::setValue);
    connect(m_device, &Device::ch1DescriptionChanged, ch1ThresholdDial, &CustomDial::setDescription);

    CustomDial * const ch2ThresholdDial = new CustomDial(tr("LVL.2"));
    ch2ThresholdDial->setFont(font);
    ch2ThresholdDial->setRange(0, 255);
    ch2ThresholdDial->setValue(128);
    connect(ch2ThresholdDial, &CustomDial::valueChanged, m_device, &Device::setCh2Threshold);
    connect(m_device, &Device::ch2ThresholdChanged, ch2ThresholdDial, &CustomDial::setValue);
    connect(m_device, &Device::ch2DescriptionChanged, ch2ThresholdDial, &CustomDial::setDescription);

    QGridLayout * const layout = new QGridLayout(this);

    layout->addWidget(ch1CouplingButton, 0, 0);
    layout->addWidget(ch1ProbeButton, 1, 0);
    layout->addWidget(ch1ThresholdDial, 0, 1, 2, 2);

    layout->addWidget(ch2CouplingButton, 0, 10);
    layout->addWidget(ch2ProbeButton, 1, 10);
    layout->addWidget(ch2ThresholdDial, 0, 11, 2, 2);

    layout->addWidget(clockButton, 0, 5);
    //layout->addStretch();
}

void MainWindow::upda()
{

}

