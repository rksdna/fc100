#include <QMenu>
#include <QMenuBar>
#include <QSettings>
#include "Device.h"
#include "DeviceWidget.h"
#include "MainWindow.h"
#include "PortDialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      m_device(new HardwareDevice(this))
{
    QMenu * const fileMenu = menuBar()->addMenu(tr("File"));

    QAction * const deviceAction = fileMenu->addAction(tr("Device..."));
    connect(deviceAction, &QAction::triggered, this, &MainWindow::openPortDialog);

    fileMenu->addSeparator();

    QAction * const quitAction = fileMenu->addAction(tr("Quit"));
    quitAction->setShortcut(QKeySequence::Quit);
    connect(quitAction, &QAction::triggered, this, &MainWindow::close);

    setCentralWidget(new DeviceWidget(m_device));

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
    m_device->connectToDevice(PortDialog::portName());
}

void MainWindow::openPortDialog()
{
    PortDialog * const dialog = new PortDialog(this);
    connect(dialog, &PortDialog::finished, dialog, &PortDialog::deleteLater);
    connect(dialog, &PortDialog::accepted, this, &MainWindow::restartDevice);
    dialog->open();
}
