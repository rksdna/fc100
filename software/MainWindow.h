#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class Device;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);

protected:
    void closeEvent(QCloseEvent *event);

private:
    void restartDevice();
    void openPortDialog();

private:
    Device * const m_device;
};

#endif // MAINWINDOW_H
