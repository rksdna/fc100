#include <QComboBox>
#include <QFormLayout>
#include <QDoubleSpinBox>
#include "Device.h"
#include "DeviceWidget.h"

DeviceWidget::DeviceWidget(Device *device, QWidget *parent)
    : QFrame(parent),
      m_device(device)
{
    QComboBox * const ch1Box = new QComboBox;
    ch1Box->addItem(tr("DC"), Device::DcCompling);
    ch1Box->addItem(tr("AC"), Device::AcCoupling);

    QComboBox * const ch2Box = new QComboBox;
    ch2Box->addItem(tr("DC"), Device::DcCompling);
    ch2Box->addItem(tr("AC"), Device::AcCoupling);

    QDoubleSpinBox * const ch1ThresholdBox = new QDoubleSpinBox;
    ch1ThresholdBox->setRange(-5.0, 5.0);

    QDoubleSpinBox * const ch2ThresholdBox = new QDoubleSpinBox;
    ch2ThresholdBox->setRange(-5.0, 5.0);

    QFormLayout * const layout = new QFormLayout(this);

    layout->addRow(tr("Ch1 threshold"), ch1ThresholdBox);
    layout->addRow(tr("Ch1 mode"), ch1Box);
    layout->addRow(tr("Ch2 threshold"), ch2ThresholdBox);
    layout->addRow(tr("Ch1 mode"), ch2Box);

    updateDevice();
}

void DeviceWidget::updateDevice()
{

}
