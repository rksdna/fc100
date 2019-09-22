#include <QMenu>
#include <QDate>
#include <QDebug>
#include <QMenuBar>
#include <QSettings>
#include <QMessageBox>
#include <QApplication>
#include "Device.h"
#include "MainWindow.h"
#include "DeviceWidget.h"
#include "PreferencesDialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      m_device(new Device(this))
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
    settings.beginGroup("MainWindow");
    restoreState(settings.value("State").toByteArray());
    restoreGeometry(settings.value("Geometry").toByteArray());
    settings.endGroup();

    settings.beginGroup("Device");
    m_device->restoreFromSettings(settings);
    settings.endGroup();

    m_device->open();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    QSettings settings;
    settings.beginGroup("MainWindow");
    settings.setValue("State", saveState());
    settings.setValue("Geometry", saveGeometry());
    settings.endGroup();

    settings.beginGroup("Device");
    m_device->saveToSettings(settings);
    settings.endGroup();

    QMainWindow::closeEvent(event);
}

void MainWindow::preferences()
{
    PreferencesDialog * const dialog = new PreferencesDialog(m_device, this);
    connect(dialog, &PreferencesDialog::finished, dialog, &QObject::deleteLater);

    dialog->open();
}

void MainWindow::about()
{
    const QString text = tr("<b>%1</b><br>%2<br><br>Copyright © %3 rksdna, murych<br>The program is provided under MIT license<br>")
            .arg(QApplication::applicationDisplayName())
            .arg(QApplication::organizationDomain())
            .arg(QDate::currentDate().year());

    QMessageBox * const dialog = new QMessageBox(QMessageBox::NoIcon,
                                                 QApplication::applicationDisplayName(),
                                                 text,
                                                 QMessageBox::Ok,
                                                 this);

    dialog->setIconPixmap(QApplication::windowIcon().pixmap(48, 48));
    connect(dialog, &QMessageBox::finished, dialog, &QMessageBox::deleteLater);

    dialog->open();
}
