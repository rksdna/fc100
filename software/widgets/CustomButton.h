#ifndef CUSTOMBUTTON_H
#define CUSTOMBUTTON_H

#include <QVariant>
#include <QToolButton>

class CustomButton : public QToolButton
{
    Q_OBJECT

public:
    CustomButton(const QString &text, QObject *object, const char *property, QWidget *parent = 0);
    void addItem(const QString &text, const QVariant &data);

    QVariant data() const;
    void setData(const QVariant &data);

public slots:
    void refresh();

private:
    void changeData();
    void paintEvent(QPaintEvent *event);

private:
    QObject * const m_object;
    const char * const m_property;
};

#endif // CUSTOMBUTTON_H
