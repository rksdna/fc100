#ifndef CUSTOMDISPLAY_H
#define CUSTOMDISPLAY_H

#include <QWidget>

class CustomDisplay : public QWidget
{
    Q_OBJECT

public:
    explicit CustomDisplay(QWidget *parent = 0);

    QColor color() const;
    void setColor(const QColor &color);

    QSize sizeHint() const;
    QSize minimumSizeHint() const;

protected:
    virtual void paintContent(const QRect &content, QPainter &painter) const = 0;
    virtual QSize contentSize() const = 0;

    void paintEvent(QPaintEvent *event);

private:
    QColor m_color;
};

#endif // CUSTOMDISPLAY_H
