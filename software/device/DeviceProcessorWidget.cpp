#include <QSpinBox>
#include <QLineEdit>
#include <QComboBox>
#include <QFormLayout>
#include <QGridLayout>
#include "DeviceProcessorWidget.h"
#include "DeviceProcessorFormatWidget.h"

DeviceProcessorWidget::DeviceProcessorWidget(DeviceProcessor *processor, QWidget *parent)
    : QWidget(parent),
      m_processor(processor),
      m_modeBox(new QComboBox),
      m_sizeBox(new QSpinBox),
      m_conversionBox(new QComboBox),
      m_expressionEdit(new QLineEdit)
{
    m_widgets.append(new DeviceProcessorFormatWidget(tr("Number "), processor, DeviceProcessor::NoType));
    m_widgets.append(new DeviceProcessorFormatWidget(tr("Duty cycle format"), processor, DeviceProcessor::DutyType));
    m_widgets.append(new DeviceProcessorFormatWidget(tr("Time format"), processor, DeviceProcessor::TimeType));
    m_widgets.append(new DeviceProcessorFormatWidget(tr("Frequency format"), processor, DeviceProcessor::FrequencyType));
    m_widgets.append(new DeviceProcessorFormatWidget(tr("Custom format"), processor, DeviceProcessor::CustomType));

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
    statisticLayout->addRow(tr("Max size"), m_sizeBox);

    QGridLayout * const layout = new QGridLayout(this);

    layout->addWidget(statisticBox, 0, 0, 1, 2);
    layout->addWidget(conversionBox, 1, 0, 1, 2);

    int index = 4;
    foreach (DeviceProcessorFormatWidget *widget, m_widgets)
    {
        layout->addWidget(widget, index / 2, index % 2);
        index++;
    }
}

void DeviceProcessorWidget::accept()
{
    foreach (DeviceProcessorFormatWidget *widget, m_widgets)
        widget->accept();

    m_processor->setConversion(DeviceProcessor::Conversion(m_conversionBox->currentData().toInt()));
    m_processor->setExpression(m_expressionEdit->text());

    m_processor->setMode(DeviceProcessor::Mode(m_modeBox->currentData().toInt()));
    m_processor->setSize(m_sizeBox->value());
}
