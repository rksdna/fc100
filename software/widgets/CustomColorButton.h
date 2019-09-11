#ifndef CUSTOMCOLORBUTTON_H
#define CUSTOMCOLORBUTTON_H

#include "CustomButton.h"

class CustomColorButton : public CustomButton
{
    Q_OBJECT

public:
    explicit CustomColorButton(QWidget *parent = 0);

protected:
    void paintContent(const QRect &content, QPainter &painter) const;
    QSize contentSize() const;

private:
    void changeColor();
};

#endif // CUSTOMCOLORBUTTON_H
