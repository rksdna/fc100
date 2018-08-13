#include <QDebug>
#include <QSettings>
#include <QComboBox>
#include <QBoxLayout>
#include <QFormLayout>
#include <QSerialPortInfo>
#include <QDialogButtonBox>
#include "Device.h"
#include "PortDialog.h"

QString PortDialog::portName()
{
    QSettings settings;
    const QString name = settings.value("DeviceDialog/Port").toString();
    if (!name.isEmpty())
        return name;

    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        if (info.vendorIdentifier() == Device::vendorIdentifier() && info.productIdentifier() == Device::productIdentifier() && !info.isBusy())
            return info.systemLocation();
    }

    return QString();
}

PortDialog::PortDialog(QWidget *parent)
    : QDialog(parent),
      m_portBox(new QComboBox)
{
    m_portBox->addItem(tr("Auto"), QString());
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
        m_portBox->addItem(tr("%1 %2").arg(info.systemLocation(), info.description()), info.systemLocation());

    QFormLayout * const formLayout = new QFormLayout;
    formLayout->addRow(tr("Port"), m_portBox);

    QDialogButtonBox * const buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &PortDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &PortDialog::reject);

    QBoxLayout * const layout = new QVBoxLayout(this);
    layout->addLayout(formLayout);
    layout->addWidget(buttonBox);

    QSettings settings;
    restoreGeometry(settings.value("DeviceDialog/Geometry").toByteArray());

    m_portBox->setCurrentIndex(m_portBox->findData(settings.value("DeviceDialog/Port").toString()));
}

void PortDialog::accept()
{
    QSettings settings;
    settings.setValue("DeviceDialog/Port", m_portBox->currentData().toString());

    QDialog::accept();
}

void PortDialog::hideEvent(QHideEvent *event)
{
    QSettings settings;
    settings.setValue("DeviceDialog/Geometry", saveGeometry());

    QDialog::hideEvent(event);
}
