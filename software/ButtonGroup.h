#ifndef BUTTONGROUP_H
#define BUTTONGROUP_H

#include <QVariant>
#include <QToolButton>
#include <QButtonGroup>

class ButtonGroup : public QButtonGroup
{
    Q_OBJECT

public:
    explicit ButtonGroup(QObject *parent = 0);

    QToolButton *addDataButton(const QString &text, const QVariant &data);

    QVariant currentData() const;
    void setCurrentData(const QVariant &data);

    void setEnabled(bool enabled);

Q_SIGNALS:
    void currentDataChanged(const QVariant &data);

private:
    void updateCurrentData(QAbstractButton * button, bool checked);
};

#endif // BUTTONGROUP_H
