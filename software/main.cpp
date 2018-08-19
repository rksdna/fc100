#include <QDebug>
#include <QTranslator>
#include <QApplication>
#include <QLibraryInfo>
#include <QStandardPaths>
#include "MainWindow.h"

int main(int argc, char *argv[])
{
    qsrand(QDateTime::currentMSecsSinceEpoch());

    QApplication application(argc, argv);
    application.setApplicationName(DEPLOY_PRODUCT);
    application.setApplicationVersion(DEPLOY_VERSION);
    application.setApplicationDisplayName(QApplication::tr("%1 %2").arg(DEPLOY_PRODUCT, DEPLOY_VERSION));

    QIcon icon;
    icon.addFile(":/arts/fct_16");
    icon.addFile(":/arts/fct_24");
    icon.addFile(":/arts/fct_32");
    icon.addFile(":/arts/fct_48");
    icon.addFile(":/arts/fct_64");
    icon.addFile(":/arts/fct_72");
    icon.addFile(":/arts/fct_96");
    icon.addFile(":/arts/fct_128");

    application.setWindowIcon(icon);

    const QString locale = QLocale::system().bcp47Name();

    QTranslator qtTranslator;
    qtTranslator.load(QString("qt_%1.qm").arg(locale), QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    application.installTranslator(&qtTranslator);

    QTranslator qtBaseTranslator;
    qtBaseTranslator.load(QString("qtbase_%1.qm").arg(locale), QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    application.installTranslator(&qtBaseTranslator);

    QTranslator appTranslator;
    appTranslator.load(QStandardPaths::locate(QStandardPaths::AppDataLocation, QString("fct_%1.qm").arg(locale)));
    application.installTranslator(&appTranslator);

    MainWindow widget;
    widget.show();

    return application.exec();
}
