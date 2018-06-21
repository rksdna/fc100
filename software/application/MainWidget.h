#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QMainWindow>

class Device;

class MainWidget : public QMainWindow
{
    Q_OBJECT

public:
    MainWidget(QWidget *parent = 0);

private:
    Device * const m_device;
};

#endif // MAINWIDGET_H
