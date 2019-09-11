#include <QSpinBox>
#include <QLineEdit>
#include <QComboBox>
#include <QGroupBox>
#include <QBoxLayout>
#include <QFormLayout>
#include "Device.h"
#include "DeviceProcessor.h"
#include "DeviceProcessingWidget.h"

DeviceProcessingWidget::DeviceProcessingWidget(Device *device, QWidget *parent)
    : QWidget(parent),
      m_device(device),
      m_modeBox(new QComboBox),
      m_sizeBox(new QSpinBox),
      m_conversionBox(new QComboBox),
      m_expressionEdit(new QLineEdit)
{
    DeviceProcessor * const processor = device->processor();

    m_modeBox->addItem(tr("Sliding window"), DeviceProcessor::SlidingMode);
    m_modeBox->addItem(tr("Cumulative"), DeviceProcessor::CumulativeMode);
    m_modeBox->setCurrentIndex(m_modeBox->findData(processor->mode()));

    m_sizeBox->setRange(DeviceProcessor::minSize(), DeviceProcessor::maxSize());
    m_sizeBox->setValue(processor->size());

    m_conversionBox->addItem(tr("None"), DeviceProcessor::NoConversion);
    m_conversionBox->addItem(tr("Expression"), DeviceProcessor::ExpressionConversion);
    m_conversionBox->setCurrentIndex(m_conversionBox->findData(processor->conversion()));

    m_expressionEdit->setPlaceholderText(tr("2 * x + 1"));
    m_expressionEdit->setText(processor->expression());

    QGroupBox * const conversionBox = new QGroupBox(tr("Conversion"));
    QFormLayout * const conversionLayout = new QFormLayout(conversionBox);
    conversionLayout->addRow(tr("Mode"), m_conversionBox);
    conversionLayout->addRow(tr("Expression"), m_expressionEdit);

    QGroupBox * const statisticBox = new QGroupBox(tr("Statistics"));
    QFormLayout * const statisticLayout = new QFormLayout(statisticBox);
    statisticLayout->addRow(tr("Mode"), m_modeBox);
    statisticLayout->addRow(tr("Size"), m_sizeBox);

    QBoxLayout * const layout = new QVBoxLayout(this);
    layout->addWidget(statisticBox);
    layout->addWidget(conversionBox);
    layout->addStretch();
}

void DeviceProcessingWidget::accept()
{
    DeviceProcessor * const processor = m_device->processor();

    processor->setConversion(DeviceProcessor::Conversion(m_conversionBox->currentData().toInt()));
    processor->setExpression(m_expressionEdit->text());

    processor->setMode(DeviceProcessor::Mode(m_modeBox->currentData().toInt()));
    processor->setSize(m_sizeBox->value());
}
