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

DeviceDialog::DeviceDialog(Device *device, QWidget *parent)
    : QDialog(parent),
      m_device(device),
      m_clockBox(new QComboBox),
      m_clockFrequencyBox(new QDoubleSpinBox),
      m_timeUnitBox(new QComboBox),
      m_timeDecimalsBox(new QSpinBox),
      m_frequencyUnitBox(new QComboBox),
      m_frequencyDecimalsBox(new QSpinBox),
      m_functionEdit(new QLineEdit),
      m_functionUnitEdit(new QLineEdit),
      m_functionDecimalsBox(new QSpinBox),
      m_portNameBox(new QComboBox)
{
    m_clockBox->addItem(tr("Internal"), Device::InternalClock);
    m_clockBox->addItem(tr("External"), Device::ExternalClock);
    m_clockBox->setCurrentIndex(m_clockBox->findData(device->clock()));

    m_clockFrequencyBox->setSuffix(tr(" Hz"));
    m_clockFrequencyBox->setRange(9000000.0, 11000000.0);
    m_clockFrequencyBox->setDecimals(3);
    m_clockFrequencyBox->setValue(device->reference());

    m_timeUnitBox->addItem(tr("nS"), Device::NanoSecond);
    m_timeUnitBox->addItem(tr("uS"), Device::MicroSecond);
    m_timeUnitBox->addItem(tr("mS"), Device::MilliSecond);
    m_timeUnitBox->addItem(tr("S"), Device::Second);
    m_timeUnitBox->setCurrentIndex(m_timeUnitBox->findData(device->timeUnit()));

    m_timeDecimalsBox->setRange(0, 9);
    m_timeDecimalsBox->setValue(device->timeDecimals());

    m_frequencyUnitBox->addItem(tr("Hz"), Device::Hetz);
    m_frequencyUnitBox->addItem(tr("kHz"), Device::KiloHertz);
    m_frequencyUnitBox->addItem(tr("MHz"), Device::MegaHertz);
    m_frequencyUnitBox->addItem(tr("GHz"), Device::GigaHertz);
    m_frequencyUnitBox->setCurrentIndex(m_frequencyUnitBox->findData(device->frequencyUnit()));

    m_frequencyDecimalsBox->setRange(0, 9);
    m_frequencyDecimalsBox->setValue(device->frequencyDecimals());

    m_functionEdit->setText(device->function());

    m_functionUnitEdit->setText(device->functionUnit());

    m_functionDecimalsBox->setRange(0, 9);
    m_functionDecimalsBox->setValue(device->functionDecimals());

    m_portNameBox->addItem(tr("Auto"), QString());
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
        m_portNameBox->addItem(tr("%1 %2").arg(info.systemLocation(), info.description()), info.systemLocation());

    m_portNameBox->setCurrentIndex(m_portNameBox->findData(device->portName()));

    QWidget * const refWidget = new QWidget;
    QFormLayout * const refLayout = new QFormLayout(refWidget);
    refLayout->addRow(tr("Clock source"), m_clockBox);
    refLayout->addRow(tr("Clock frequency"), m_clockFrequencyBox);

    QWidget * const preferencesWidget = new QWidget;
    QFormLayout * const preferencesLayout = new QFormLayout(preferencesWidget);
    preferencesLayout->addRow(tr("Time unit"), m_timeUnitBox);
    preferencesLayout->addRow(tr("Time precision"), m_timeDecimalsBox);

    preferencesLayout->addRow(tr("Frequency unit"), m_frequencyUnitBox);
    preferencesLayout->addRow(tr("Frequency precision"), m_frequencyDecimalsBox);

    preferencesLayout->addRow(tr("Function"), m_functionEdit);
    preferencesLayout->addRow(tr("Function unit"), m_functionUnitEdit);
    preferencesLayout->addRow(tr("Function precision"), m_functionDecimalsBox);

    QWidget * const connectionWidget = new QWidget;
    QFormLayout * const connectionLayout = new QFormLayout(connectionWidget);
    connectionLayout->addRow(tr("Port"), m_portNameBox);

    QTabWidget * const tabWidget = new QTabWidget;
    tabWidget->addTab(preferencesWidget, tr("Preferences"));
    tabWidget->addTab(refWidget, tr("Clock"));
    tabWidget->addTab(connectionWidget, tr("Connection"));

    QDialogButtonBox * const buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &DeviceDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &DeviceDialog::reject);

    QBoxLayout * const layout = new QVBoxLayout(this);
    layout->addWidget(tabWidget);
    layout->addWidget(buttonBox);

    QSettings settings;
    restoreGeometry(settings.value("DeviceDialog/Geometry").toByteArray());
}

void DeviceDialog::accept()
{
    m_device->setClock(Device::Clock(m_clockBox->currentData().toInt()));
    m_device->setReference(m_clockFrequencyBox->value());
    m_device->setTimeUnit(Device::TimeUnit(m_timeUnitBox->currentData().toInt()));
    m_device->setTimeDecimals(m_timeDecimalsBox->value());
    m_device->setFrequencyUnit(Device::FrequencyUnit(m_frequencyUnitBox->currentData().toInt()));
    m_device->setFrequencyDecimals(m_frequencyDecimalsBox->value());
    m_device->setFunction(m_functionEdit->text());
    m_device->setFunctionUnit(m_functionUnitEdit->text());
    m_device->setFunctionDecimals(m_functionDecimalsBox->value());
    m_device->setPortName(m_portNameBox->currentData().toString());

    QDialog::accept();
}

void DeviceDialog::hideEvent(QHideEvent *event)
{
    QSettings settings;
    settings.setValue("DeviceDialog/Geometry", saveGeometry());

    QDialog::hideEvent(event);
}
