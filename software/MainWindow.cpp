#include <QMenu>
#include <QMenuBar>
#include <QSettings>
#include "Device.h"
#include "MainWindow.h"
#include "DeviceWidget.h"

MainWindow::MainWindow(const QString &type, QWidget *parent)
    : QMainWindow(parent),
      m_device(Device::createDevice(type, this))
{
    setCentralWidget(new DeviceWidget(m_device));

    QMenu * const deviceMenu = menuBar()->addMenu(tr("Device"));

    QAction * const preferencesAction = deviceMenu->addAction(tr("Preferences..."));
    preferencesAction->setShortcut(QKeySequence::Preferences);
    connect(preferencesAction, &QAction::triggered, this, &MainWindow::preferences);

    deviceMenu->addSeparator();

    QAction * const quitAction = deviceMenu->addAction(tr("Quit"));
    quitAction->setShortcut(QKeySequence::Quit);
    connect(quitAction, &QAction::triggered, this, &MainWindow::close);

    QMenu * const helpMenu = menuBar()->addMenu(tr("Help"));

    QAction * const aboutAction = helpMenu->addAction(tr("About..."));
    connect(aboutAction, &QAction::triggered, this, &MainWindow::about);

    QSettings settings;
    restoreState(settings.value("MainWindow/State").toByteArray());
    restoreGeometry(settings.value("MainWindow/Geometry").toByteArray());

    m_device->reset();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    QSettings settings;
    settings.setValue("MainWindow/State", saveState());
    settings.setValue("MainWindow/Geometry", saveGeometry());

    QMainWindow::closeEvent(event);
}

void MainWindow::preferences()
{
}

void MainWindow::about()
{
}
