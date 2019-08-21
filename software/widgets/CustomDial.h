#ifndef CUSTOMDIAL_H
#define CUSTOMDIAL_H

#include <QDial>
#include <QVariant>

class CustomDial : public QDial
{
    Q_OBJECT

public:
    explicit CustomDial(const QString &text, QObject *object, const char *property, QWidget *parent = 0);

    QVariant data() const;
    void setData(const QVariant &data);

public slots:
    void refresh();

private:
    void changeData();

    void initStyleOption(QStyleOptionSlider *option) const;

    void paintEvent(QPaintEvent *event);


private:
    const QString m_text;
    QObject * const m_object;
    const char * const m_property;
};

#endif // CUSTOMDIAL_H
