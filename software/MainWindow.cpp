#include <QMenu>
#include <QDate>
#include <QDebug>
#include <QMenuBar>
#include <QSettings>
#include <QFileDialog>
#include <QMessageBox>
#include <QApplication>
#include "Device.h"
#include "Settings.h"
#include "MainWindow.h"
#include "DeviceWidget.h"
#include "DeviceProcessor.h"
#include "PreferencesDialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      m_device(new Device(this))
{
    setCentralWidget(new DeviceWidget(m_device));

    QMenu * const deviceMenu = menuBar()->addMenu(tr("Device"));

    QAction * const saveAsAction = deviceMenu->addAction(tr("Save as..."));
    saveAsAction->setShortcut(QKeySequence::SaveAs);
    connect(saveAsAction, &QAction::triggered, this, &MainWindow::saveAs);

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

    QSettings settings(settingsPath(), QSettings::IniFormat);

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
    QSettings settings(settingsPath(), QSettings::IniFormat);

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

void MainWindow::saveAs()
{
    QFileDialog * const dialog = new QFileDialog(this);
    dialog->setWindowTitle(QApplication::applicationDisplayName());
    dialog->setDefaultSuffix("csv");
    dialog->setNameFilter("CSV files (*.csv)");
    dialog->setAcceptMode(QFileDialog::AcceptSave);

    connect(dialog, &QFileDialog::fileSelected, m_device->processor(), &DeviceProcessor::saveToFile);
    connect(dialog, &QFileDialog::finished, dialog, &QFileDialog::deleteLater);
    dialog->open();
}

void MainWindow::about()
{
    const QString text = tr("<p><b>%1</b></p><p>Copyright © %3 rksdna, murych</p><p>The program is provided under MIT license</p>")
            .arg(QApplication::applicationDisplayName())
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
