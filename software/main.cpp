#include <QIcon>
#include <QDebug>
#include <QTranslator>
#include <QApplication>
#include <QLibraryInfo>
#include <QStandardPaths>
#include <QCommandLineParser>
#include "DeviceWindow.h"

int main(int argc, char *argv[])
{
    qsrand(QDateTime::currentMSecsSinceEpoch());

    QApplication application(argc, argv);
    application.setApplicationName(DEPLOY_PRODUCT);
    application.setApplicationVersion(DEPLOY_VERSION);
    application.setApplicationDisplayName(QString("%1 %2").arg(DEPLOY_PRODUCT, DEPLOY_VERSION));

    QIcon icon;
    icon.addFile(":/arts/fc100_16");
    icon.addFile(":/arts/fc100_24");
    icon.addFile(":/arts/fc100_32");
    icon.addFile(":/arts/fc100_48");
    icon.addFile(":/arts/fc100_64");
    icon.addFile(":/arts/fc100_72");
    icon.addFile(":/arts/fc100_96");
    icon.addFile(":/arts/fc100_128");

    application.setWindowIcon(icon);

    const QString locale = QLocale::system().bcp47Name();

    QTranslator qtTranslator;
    qtTranslator.load(QString("qt_%1.qm").arg(locale), QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    application.installTranslator(&qtTranslator);

    QTranslator qtBaseTranslator;
    qtBaseTranslator.load(QString("qtbase_%1.qm").arg(locale), QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    application.installTranslator(&qtBaseTranslator);

    QTranslator appTranslator;
    appTranslator.load(QStandardPaths::locate(QStandardPaths::AppDataLocation, QString("fc100_%1.qm").arg(locale)));
    application.installTranslator(&appTranslator);

    QCommandLineParser parser;
    parser.setApplicationDescription(QApplication::tr("Frequency counter"));
    parser.addVersionOption();
    parser.addHelpOption();
    parser.addOption(QCommandLineOption("device", QApplication::tr("Select device class"), "device", "fc100"));

    parser.process(application);

    DeviceWindow window;
    window.show();

    return application.exec();
}
