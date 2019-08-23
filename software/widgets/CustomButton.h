#ifndef CUSTOMBUTTON_H
#define CUSTOMBUTTON_H

#include <QMap>
#include <QAbstractButton>

class CustomButton : public QAbstractButton
{
    Q_OBJECT

public:
    explicit CustomButton(const QString &text, QWidget *parent = 0);

    int value() const;
    void setValue(int value);
    void addValue(const QString &text, int value);

    QSize sizeHint() const;
    QSize minimumSizeHint() const;

signals:
    void valueChanged(int value);

protected:
    void paintEvent(QPaintEvent *event);

private:
    void initStyleOption(class QStyleOptionButton *option) const;
    void changeValue();

private:
    QMap<int, QString> m_items;
    QMargins m_margins;
    int m_value;
};

#endif // CUSTOMBUTTON_H
