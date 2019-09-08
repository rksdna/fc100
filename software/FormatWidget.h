#ifndef FORMATWIDGET_H
#define FORMATWIDGET_H

#include <QGroupBox>

/*class QSpinBox;
class QComboBox;
class Sample::Format;

class FormatWidget : public QGroupBox
{
    Q_OBJECT

public:
    explicit FormatWidget(const QString &title, Sample::Format * format, QWidget *parent = 0);

    void accept();

private:
    Sample::Format * const m_format;
    QComboBox * const m_unitBox;
    QSpinBox * const m_decimalsBox;
};*/

#endif // FORMATWIDGET_H
/*
 *
 *     m_referenceSourceBox->addItem(tr("Internal source"), DeviceReference::InternalSource);
    m_referenceSourceBox->addItem(tr("External source (REF)"), DeviceReference::ExternalSource);
    m_referenceSourceBox->setCurrentIndex(m_referenceSourceBox->findData(device->reference()->source()));

    m_referenceFrequencyBox->setRange(DeviceReference::minFrequency(), DeviceReference::maxFrequency());
    m_referenceFrequencyBox->setDecimals(3);
    m_referenceFrequencyBox->setValue(device->reference()->frequency());

    QWidget * const referenceWidget = new QWidget;
    QFormLayout * const referenceLayout = new QFormLayout(referenceWidget);
    referenceLayout->addRow(tr("Source"), m_referenceSourceBox);
    referenceLayout->addRow(tr("Frequency, Hz"), m_referenceFrequencyBox);

    QWidget * const formatsWidget = new QWidget;
    QFormLayout * const formatsLayout = new QFormLayout(formatsWidget);
    formatsLayout->addWidget(m_countFormatWidget);
    formatsLayout->addWidget(m_timeFormatWidget);
    formatsLayout->addWidget(m_frequencyFormatWidget);
    formatsLayout->addWidget(m_userFormatWidget);

    //...
    m_functionEdit->setText(device->function());

    m_portNameBox->addItem(tr("Auto"), QString());
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
        m_portNameBox->addItem(tr("%1 %2").arg(info.systemLocation(), info.description()), info.systemLocation());

    m_portNameBox->setCurrentIndex(m_portNameBox->findData(device->portName()));

    QWidget * const connectionWidget = new QWidget;
    QFormLayout * const connectionLayout = new QFormLayout(connectionWidget);
    connectionLayout->addRow(tr("Port"), m_portNameBox);*/
