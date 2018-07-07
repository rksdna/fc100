#ifndef PORTDIALOG_H
#define PORTDIALOG_H

#include <QDialog>

class QComboBox;

class PortDialog : public QDialog
{
    Q_OBJECT

public:
    static QString portName();

public:
    explicit PortDialog(QWidget *parent = 0);
    void accept();

protected:
    void hideEvent(QHideEvent *event);

private:
    QComboBox * const m_portBox;
};

#endif // PORTDIALOG_H
