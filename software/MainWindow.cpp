#include <QMenu>
#include <QMenuBar>
#include <QDateTime>
#include <QSettings>
#include <QMessageBox>
#include <QApplication>
#include "Device.h"
#include "DeviceWidget.h"
#include "MainWindow.h"
#include "PortDialog.h"

MainWindow::MainWindow(const QString &device, QWidget *parent)
    : QMainWindow(parent),
      m_device(Device::create(device, this))
{
    QMenu * const fileMenu = menuBar()->addMenu(tr("File"));

    QAction * const deviceAction = fileMenu->addAction(tr("Device..."));
    connect(deviceAction, &QAction::triggered, this, &MainWindow::openPortDialog);

    fileMenu->addSeparator();

    QAction * const quitAction = fileMenu->addAction(tr("Quit"));
    quitAction->setShortcut(QKeySequence::Quit);
    connect(quitAction, &QAction::triggered, this, &MainWindow::close);

    QMenu * const helpMenu = menuBar()->addMenu(tr("Help"));

    QAction * const aboutAction = helpMenu->addAction(tr("About..."));
    connect(aboutAction, &QAction::triggered, this, &MainWindow::about);

    setCentralWidget(new DeviceWidget(m_device));

    QSettings settings;
    restoreState(settings.value("MainWindow/State").toByteArray());
    restoreGeometry(settings.value("MainWindow/Geometry").toByteArray());

    restartDevice();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    QSettings settings;
    settings.setValue("MainWindow/State", saveState());
    settings.setValue("MainWindow/Geometry", saveGeometry());

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

void MainWindow::about()
{
    const QString text = tr("%1")
            .arg(QApplication::applicationDisplayName());

    QMessageBox * const dialog = new QMessageBox(QMessageBox::NoIcon,
                                                 QApplication::applicationDisplayName(),
                                                 text,
                                                 QMessageBox::Ok,
                                                 this);

    dialog->setIconPixmap(QApplication::windowIcon().pixmap(48));

    connect(dialog, &QMessageBox::finished, dialog, &QMessageBox::deleteLater);

    dialog->open();
}
