#ifndef PREFERENCESDIALOG_H
#define PREFERENCESDIALOG_H

#include <QDialog>

class Device;

class PreferencesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PreferencesDialog(Device *device, QWidget *parent = 0);

protected:
    void hideEvent(QHideEvent *event);
};

#endif // PREFERENCESDIALOG_H

