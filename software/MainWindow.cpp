#include <QBoxLayout>
#include "MainWindow.h"
#include "Device.h"
#include "CustomButton.h"
#include "CustomDial.h"

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent),
      m_device(new Device(this))
{
    CustomButton * const ch1CouplingButton = new CustomButton(tr("CH1"), m_device, "ch1Coupling");
    ch1CouplingButton->addItem(tr("DC"), Device::DcCoupling);
    ch1CouplingButton->addItem(tr("AC"), Device::AcCoupling);
    ch1CouplingButton->refresh();

    CustomButton * const ch2CouplingButton = new CustomButton(tr("CH2"), m_device, "ch2Coupling");
    ch2CouplingButton->addItem(tr("DC"), Device::DcCoupling);
    ch2CouplingButton->addItem(tr("AC"), Device::AcCoupling);
    ch2CouplingButton->refresh();

    CustomButton * const ch1ProbeButton = new CustomButton(tr("CH1"), m_device, "ch1Probe");
    ch1ProbeButton->addItem(tr("1:1"), Device::x1Probe);
    ch1ProbeButton->addItem(tr("1:10"), Device::x10Probe);
    ch1ProbeButton->addItem(tr("1:100"), Device::x100Probe);
    ch1ProbeButton->refresh();

    CustomButton * const ch2ProbeButton = new CustomButton(tr("CH2"), m_device, "ch2Probe");
    ch2ProbeButton->addItem(tr("1:1"), Device::x1Probe);
    ch2ProbeButton->addItem(tr("1:10"), Device::x10Probe);
    ch2ProbeButton->addItem(tr("1:100"), Device::x100Probe);
    ch2ProbeButton->refresh();

    CustomButton * const cb3 = new CustomButton(tr("REF"), m_device, "clock");
    cb3->addItem(tr("INT"), Device::InternalClock);
    cb3->addItem(tr("EXT"), Device::ExternalClock);
    cb3->refresh();

    CustomDial * const td1 = new CustomDial(tr("CH1"), m_device, "ch1Threshold");
    td1->setRange(0, 255);
    td1->refresh();

    CustomDial * const td2 = new CustomDial(tr("CH2"), m_device, "ch1Threshold");
    td2->setRange(0, 255);
    td2->refresh();



    QBoxLayout * const layout = new QVBoxLayout(this);
    layout->addWidget(ch1CouplingButton);
    layout->addWidget(ch2CouplingButton);
    layout->addWidget(ch1ProbeButton);
    layout->addWidget(ch2ProbeButton);
    layout->addWidget(td1);
    layout->addWidget(td2);

    layout->addWidget(cb3);
    layout->addStretch();
}

