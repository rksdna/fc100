#include <QMenu>
#include <QMenuBar>
#include <QSettings>
#include <QFormLayout>
#include <QDoubleSpinBox>
#include "Device.h"
#include "MainWindow.h"
#include "PortDialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      m_device(new Device(this))
{
    QMenu * const fileMenu = menuBar()->addMenu(tr("File"));

    QAction * const deviceAction = fileMenu->addAction(tr("Device..."));
    connect(deviceAction, &QAction::triggered, this, &MainWindow::openPortDialog);

    fileMenu->addSeparator();

    QAction * const quitAction = fileMenu->addAction(tr("Quit"));
    connect(quitAction, &QAction::triggered, this, &MainWindow::close);


    QDoubleSpinBox * const ch1ThresholdBox = new QDoubleSpinBox;
    ch1ThresholdBox->setRange(-5.0, 5.0);

    QDoubleSpinBox * const ch2ThresholdBox = new QDoubleSpinBox;
    ch2ThresholdBox->setRange(-5.0, 5.0);


    QWidget * const widget = new QWidget;
    QFormLayout * const layout = new QFormLayout(widget);

    layout->addRow(tr("Ch1 threshold"), ch1ThresholdBox);
    layout->addRow(tr("Ch2 threshold"), ch2ThresholdBox);

    QSettings settings;
    restoreGeometry(settings.value("MainWindow/Geometry").toByteArray());
    restoreState(settings.value("MainWindow/State").toByteArray());

    restartDevice();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    QSettings settings;
    settings.setValue("MainWindow/Geometry", saveGeometry());
    settings.setValue("MainWindow/State", saveState());

    QMainWindow::closeEvent(event);
}

void MainWindow::restartDevice()
{
    m_device->restart(PortDialog::portName());
}

void MainWindow::openPortDialog()
{
    PortDialog * const dialog = new PortDialog(this);
    connect(dialog, &PortDialog::finished, dialog, &PortDialog::deleteLater);
    connect(dialog, &PortDialog::accepted, this, &MainWindow::restartDevice);
    dialog->open();
}
