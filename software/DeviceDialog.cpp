#include <QSpinBox>
#include <QLineEdit>
#include <QComboBox>
#include <QSettings>
#include <QTabWidget>
#include <QBoxLayout>
#include <QFormLayout>
#include <QDoubleSpinBox>
#include <QSerialPortInfo>
#include <QDialogButtonBox>
#include "Device.h"
#include "DeviceDialog.h"
#include "DeviceReference.h"
#include "DeviceProcessor.h"

/*FormatWidget::FormatWidget(const QString &title, Format *format, QWidget *parent)
    : QGroupBox(title, parent),
      m_format(format),
      m_unitBox(new QComboBox),
      m_decimalsBox(new QSpinBox)
{
    m_unitBox->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    m_unitBox->addItem(format->suffix(Format::NanoUnit), Format::NanoUnit);
    m_unitBox->addItem(format->suffix(Format::MicroUnit), Format::MicroUnit);
    m_unitBox->addItem(format->suffix(Format::MilliUnit), Format::MilliUnit);
    m_unitBox->addItem(format->suffix(Format::OneUnit), Format::OneUnit);
    m_unitBox->addItem(format->suffix(Format::KiloUnit), Format::KiloUnit);
    m_unitBox->addItem(format->suffix(Format::MegaUnit), Format::MegaUnit);
    m_unitBox->addItem(format->suffix(Format::GigaUnit), Format::GigaUnit);
    m_unitBox->setCurrentIndex(m_unitBox->findData(format->unit()));

    m_decimalsBox->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    m_decimalsBox->setRange(Format::minDecimals(), Format::maxDecimals());
    m_decimalsBox->setValue(format->decimals());

    QFormLayout * const layout = new QFormLayout(this);
    layout->addRow(tr("Unit"), m_unitBox);
    layout->addRow(tr("Decimals"), m_decimalsBox);
}

void FormatWidget::accept()
{
    m_format->setUnit(Format::Unit(m_unitBox->currentData().toInt()));
    m_format->setDecimals(m_decimalsBox->value());
}*/

DeviceDialog::DeviceDialog(Device *device, QWidget *parent)
    : QDialog(parent),
      m_device(device),
      m_referenceSourceBox(new QComboBox),
      m_referenceFrequencyBox(new QDoubleSpinBox),
      /*m_countFormatWidget(new FormatWidget(tr("Count"), device->processor()->countFormat())),
      m_timeFormatWidget(new FormatWidget(tr("Time"), device->processor()->timeFormat())),
      m_frequencyFormatWidget(new FormatWidget(tr("Frequency"), device->processor()->frequencyFormat())),
      m_userFormatWidget(new FormatWidget(tr("User"), device->processor()->userFormat())),*/
      //...
      m_functionEdit(new QLineEdit),
      m_portNameBox(new QComboBox)
{
    m_referenceSourceBox->addItem(tr("Internal source"), DeviceReference::InternalSource);
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
    /*formatsLayout->addWidget(m_countFormatWidget);
    formatsLayout->addWidget(m_timeFormatWidget);
    formatsLayout->addWidget(m_frequencyFormatWidget);
    formatsLayout->addWidget(m_userFormatWidget);*/

    //...
    m_functionEdit->setText(device->function());

    m_portNameBox->addItem(tr("Auto"), QString());
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
        m_portNameBox->addItem(tr("%1 %2").arg(info.systemLocation(), info.description()), info.systemLocation());

    m_portNameBox->setCurrentIndex(m_portNameBox->findData(device->portName()));

    QWidget * const connectionWidget = new QWidget;
    QFormLayout * const connectionLayout = new QFormLayout(connectionWidget);
    connectionLayout->addRow(tr("Port"), m_portNameBox);

    QTabWidget * const tabWidget = new QTabWidget;
    tabWidget->addTab(referenceWidget, tr("Reference"));
    tabWidget->addTab(formatsWidget, tr("Formats"));
    tabWidget->addTab(connectionWidget, tr("Connection"));

    QDialogButtonBox * const buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &DeviceDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &DeviceDialog::reject);

    QBoxLayout * const layout = new QVBoxLayout(this);
    layout->addWidget(tabWidget);
    layout->addWidget(buttonBox);

    QSettings settings;
    settings.beginGroup("DeviceDialog");
    restoreGeometry(settings.value("Geometry").toByteArray());
    settings.endGroup();
}

void DeviceDialog::accept()
{
    m_device->reference()->setSource(DeviceReference::Source(m_referenceSourceBox->currentData().toInt()));
    m_device->reference()->setFrequency(m_referenceFrequencyBox->value());

    /*m_countFormatWidget->accept();
    m_timeFormatWidget->accept();
    m_frequencyFormatWidget->accept();
    m_userFormatWidget->accept();*/

    //...
    /*m_device->setTimeUnit(Device::TimeUnit(m_timeUnitBox->currentData().toInt()));
    m_device->setTimeDecimals(m_timeDecimalsBox->value());
    m_device->setFrequencyUnit(Device::FrequencyUnit(m_frequencyUnitBox->currentData().toInt()));
    m_device->setFrequencyDecimals(m_frequencyDecimalsBox->value());
    m_device->setFunction(m_functionEdit->text());
    m_device->setFunctionUnit(m_functionUnitEdit->text());
    m_device->setFunctionDecimals(m_functionDecimalsBox->value());
    m_device->setPortName(m_portNameBox->currentData().toString());*/

    QDialog::accept();
}

void DeviceDialog::hideEvent(QHideEvent *event)
{
    QSettings settings;
    settings.beginGroup("DeviceDialog");
    settings.setValue("Geometry", saveGeometry());
    settings.endGroup();

    QDialog::hideEvent(event);
}

