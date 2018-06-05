#include <QApplication>
#include "MainWidget.h"

int main(int argc, char *argv[])
{
    QApplication application(argc, argv);
    application.setApplicationName("fs");
    application.setOrganizationDomain("www.frontol.ru");
    application.setApplicationVersion(DEPLOY_VERSION);
    application.setApplicationDisplayName(QApplication::tr("Frequency counter %1").arg(DEPLOY_VERSION));

    MainWidget widget;
    widget.show();

    return application.exec();
}
