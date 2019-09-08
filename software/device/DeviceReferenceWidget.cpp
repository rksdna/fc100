#include <QComboBox>
#include <QFormLayout>
#include <QDoubleSpinBox>
#include "DeviceReference.h"
#include "DeviceReferenceWidget.h"

DeviceReferenceWidget::DeviceReferenceWidget(DeviceReference *reference, QWidget *parent)
    : QWidget(parent),
      m_reference(reference),
      m_sourceBox(new QComboBox),
      m_frequencyBox(new QDoubleSpinBox)
{
    m_sourceBox->addItem(tr("Internal source"), DeviceReference::InternalSource);
    m_sourceBox->addItem(tr("External source (REF)"), DeviceReference::ExternalSource);
    m_sourceBox->setCurrentIndex(m_sourceBox->findData(reference->source()));

    m_frequencyBox->setRange(DeviceReference::minFrequency(), DeviceReference::maxFrequency());
    m_frequencyBox->setDecimals(3);
    m_frequencyBox->setValue(reference->frequency());

    QFormLayout * const layout = new QFormLayout(this);
    layout->addRow(tr("Source"), m_sourceBox);
    layout->addRow(tr("Frequency, Hz"), m_frequencyBox);
}

void DeviceReferenceWidget::accept()
{
    m_reference->setSource(DeviceReference::Source(m_sourceBox->currentData().toInt()));
    m_reference->setFrequency(m_frequencyBox->value());
}
