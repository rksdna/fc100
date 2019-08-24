#ifndef CUSTOMOPTIONBUTTON_H
#define CUSTOMOPTIONBUTTON_H

#include <QMap>
#include "CustomButton.h"

class CustomOptionButton : public CustomButton
{
    Q_OBJECT

public:
    explicit CustomOptionButton(const QString &title, QWidget *parent = 0);

    void addValueOption(const QString &text, int value);

    int value() const;
    void setValue(int value);

signals:
    void valueChanged(int value);

protected:
    void paintContent(const QRect &content, QPainter &painter) const;
    QSize contentSize() const;

private:
    void changeValue();

private:
    QMap<int, QString> m_options;
    int m_value;
};

#endif // CUSTOMOPTIONBUTTON_H
