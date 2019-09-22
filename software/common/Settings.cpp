#include <QDir>
#include <QApplication>
#include <QStandardPaths>
#include "Settings.h"

QString settingsPath()
{
    QDir dir(QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation));
    if (!dir.exists())
        dir.mkpath(dir.absolutePath());

    return dir.filePath(QString("%1.conf").arg(QApplication::applicationName()));
}
