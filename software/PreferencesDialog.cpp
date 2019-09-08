#include <QSettings>
#include <QTabWidget>
#include <QBoxLayout>
#include <QDialogButtonBox>
#include "Device.h"
#include "PreferencesDialog.h"
#include "DeviceProcessorWidget.h"
#include "DeviceReferenceWidget.h"

PreferencesDialog::PreferencesDialog(Device *device, QWidget *parent)
    : QDialog(parent)
{
    DeviceReferenceWidget * const referenceWidget = new DeviceReferenceWidget(device->reference());
    connect(this, &QDialog::accepted, referenceWidget, &DeviceReferenceWidget::accept);

    DeviceProcessorWidget * const processorWidget = new DeviceProcessorWidget(device->processor());
    connect(this, &QDialog::accepted, processorWidget, &DeviceProcessorWidget::accept);

    QTabWidget * const tabWidget = new QTabWidget;
    tabWidget->addTab(referenceWidget, tr("Reference"));
    tabWidget->addTab(processorWidget, tr("Processing"));

    QDialogButtonBox * const buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &PreferencesDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &PreferencesDialog::reject);

    QBoxLayout * const layout = new QVBoxLayout(this);
    layout->addWidget(tabWidget);
    layout->addWidget(buttonBox);

    QSettings settings;
    settings.beginGroup("DeviceDialog");
    restoreGeometry(settings.value("Geometry").toByteArray());
    settings.endGroup();
}

void PreferencesDialog::hideEvent(QHideEvent *event)
{
    QSettings settings;
    settings.beginGroup("DeviceDialog");
    settings.setValue("Geometry", saveGeometry());
    settings.endGroup();

    QDialog::hideEvent(event);
}

