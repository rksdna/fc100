#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>

class Device;

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);

    void upda();

private:
    Device * const m_device;
};

#endif // MAINWINDOW_H
