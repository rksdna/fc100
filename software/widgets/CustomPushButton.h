#ifndef CUSTOMPUSHBUTTON_H
#define CUSTOMPUSHBUTTON_H

#include "CustomButton.h"

class CustomPushButton : public CustomButton
{
    Q_OBJECT

public:
    explicit CustomPushButton(const QString &title, QWidget *parent = 0);

protected:
    void paintContent(const QRect &content, QPainter &painter) const;
    QSize contentSize() const;
};

#endif // CUSTOMPUSHBUTTON_H
