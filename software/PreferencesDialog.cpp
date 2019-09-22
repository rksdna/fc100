#include <QSettings>
#include <QTabWidget>
#include <QBoxLayout>
#include <QDialogButtonBox>
#include "Device.h"
#include "Settings.h"
#include "PreferencesDialog.h"
#include "DeviceProcessingWidget.h"
#include "DeviceHardwareWidget.h"
#include "DevicePreferencesWidget.h"

PreferencesDialog::PreferencesDialog(Device *device, QWidget *parent)
    : QDialog(parent)
{
    DeviceHardwareWidget * const hardwareWidget = new DeviceHardwareWidget(device);
    connect(this, &QDialog::accepted, hardwareWidget, &DeviceHardwareWidget::accept);

    DeviceProcessingWidget * const processingWidget = new DeviceProcessingWidget(device);
    connect(this, &QDialog::accepted, processingWidget, &DeviceProcessingWidget::accept);

    DevicePreferencesWidget * const preferencesWidget = new DevicePreferencesWidget(device);
    connect(this, &QDialog::accepted, preferencesWidget, &DevicePreferencesWidget::accept);

    QTabWidget * const tabWidget = new QTabWidget;
    tabWidget->addTab(hardwareWidget, tr("Hardware"));
    tabWidget->addTab(processingWidget, tr("Processing"));
    tabWidget->addTab(preferencesWidget, tr("Preferences"));

    QDialogButtonBox * const buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &PreferencesDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &PreferencesDialog::reject);

    QBoxLayout * const layout = new QVBoxLayout(this);
    layout->addWidget(tabWidget);
    layout->addWidget(buttonBox);

    QSettings settings(settingsPath(), QSettings::IniFormat);
    settings.beginGroup("DeviceDialog");
    restoreGeometry(settings.value("Geometry").toByteArray());
    settings.endGroup();
}

void PreferencesDialog::hideEvent(QHideEvent *event)
{
    QSettings settings(settingsPath(), QSettings::IniFormat);
    settings.beginGroup("DeviceDialog");
    settings.setValue("Geometry", saveGeometry());
    settings.endGroup();

    QDialog::hideEvent(event);
}

