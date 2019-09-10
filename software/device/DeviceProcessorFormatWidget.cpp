#include <QSpinBox>
#include <QComboBox>
#include <QFormLayout>
#include "Enum.h"
#include "DeviceProcessorFormatWidget.h"

DeviceProcessorFormatWidget::DeviceProcessorFormatWidget(const QString &title, DeviceProcessor *processor, DeviceProcessor::Type type, QWidget *parent)
    : QGroupBox(title, parent),
      m_processor(processor),
      m_type(type),
      m_unitBox(new QComboBox),
      m_decimalsBox(new QSpinBox)
{
    foreach (DeviceProcessor::Unit unit, knownEnum<DeviceProcessor::Unit>())
        m_unitBox->addItem(DeviceProcessor::toString(type, unit), unit);

    m_unitBox->setCurrentIndex(m_unitBox->findData(processor->unit(m_type)));

    m_decimalsBox->setRange(0, 9);
    m_decimalsBox->setValue(processor->decimals(m_type));

    QFormLayout * const layout = new QFormLayout(this);
    layout->addRow(tr("Unit"), m_unitBox);
    layout->addRow(tr("Decimals"), m_decimalsBox);
}

void DeviceProcessorFormatWidget::accept()
{
    m_processor->setUnit(m_type, DeviceProcessor::Unit(m_unitBox->currentData().toInt()));
    m_processor->setDecimals(m_type, m_decimalsBox->value());
}
