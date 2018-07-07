#include <QApplication>
#include "MainWindow.h"

int main(int argc, char *argv[])
{
    QApplication application(argc, argv);
    application.setApplicationName("fc");
    application.setOrganizationDomain("www.kovar.ru");
    application.setApplicationVersion(DEPLOY_VERSION);
    application.setApplicationDisplayName(QApplication::tr("Frequency counter %1").arg(DEPLOY_VERSION));

    MainWindow widget;
    widget.show();

    return application.exec();
}
