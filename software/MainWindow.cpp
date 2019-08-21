#include <QBoxLayout>
#include "MainWindow.h"
#include "Device.h"
#include "CustomButton.h"

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

    CustomButton * const cb3 = new CustomButton(tr("REF"), m_device, "clock");
    cb3->addItem(tr("INT"), Device::InternalClock);
    cb3->addItem(tr("EXT"), Device::ExternalClock);
    cb3->refresh();

    QBoxLayout * const layout = new QVBoxLayout(this);
    layout->addWidget(ch1CouplingButton);
    layout->addWidget(ch2CouplingButton);
    layout->addWidget(cb3);
    layout->addStretch();
}

