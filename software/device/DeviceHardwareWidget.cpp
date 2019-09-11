#include <QComboBox>
#include <QGroupBox>
#include <QFormLayout>
#include <QDoubleSpinBox>
#include <QSerialPortInfo>
#include "Device.h"
#include "DeviceReference.h"
#include "DeviceHardwareWidget.h"

DeviceHardwareWidget::DeviceHardwareWidget(Device *device, QWidget *parent)
    : QWidget(parent),
      m_device(device),
      m_portBox(new QComboBox),
      m_sourceBox(new QComboBox),
      m_frequencyBox(new QDoubleSpinBox)
{
    DeviceReference * const reference = m_device->reference();

    m_portBox->addItem(tr("Auto"), QString());
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
        m_portBox->addItem(tr("%1 %2").arg(info.systemLocation(), info.description()), info.systemLocation());

    m_portBox->setCurrentIndex(m_portBox->findData(device->portName()));

    QGroupBox * const connectionBox = new QGroupBox(tr("Connection"));
    QFormLayout * const conLayout = new QFormLayout(connectionBox);
    conLayout->addRow(tr("Port"), m_portBox);

    m_sourceBox->addItem(tr("Internal source"), DeviceReference::InternalSource);
    m_sourceBox->addItem(tr("External source (REF)"), DeviceReference::ExternalSource);
    m_sourceBox->setCurrentIndex(m_sourceBox->findData(reference->source()));

    m_frequencyBox->setRange(DeviceReference::minFrequency(), DeviceReference::maxFrequency());
    m_frequencyBox->setDecimals(3);
    m_frequencyBox->setValue(reference->frequency());

    QGroupBox * const referenceBox = new QGroupBox(tr("Reference"));
    QFormLayout * const referenceLayout = new QFormLayout(referenceBox);
    referenceLayout->addRow(tr("Source"), m_sourceBox);
    referenceLayout->addRow(tr("Frequency, Hz"), m_frequencyBox);

    QBoxLayout * const layout = new QVBoxLayout(this);
    layout->addWidget(connectionBox);
    layout->addWidget(referenceBox);
    layout->addStretch();
}

void DeviceHardwareWidget::accept()
{
    DeviceReference * const reference = m_device->reference();

    m_device->setPortName(m_portBox->currentData().toString());

    reference->setSource(DeviceReference::Source(m_sourceBox->currentData().toInt()));
    reference->setFrequency(m_frequencyBox->value());
}
